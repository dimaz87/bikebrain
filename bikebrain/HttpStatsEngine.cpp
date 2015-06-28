#include <bikebrain/HttpStatsEngine.h>

#include <stingraykit/Holder.h>
#include <stingraykit/ScopeExit.h>
#include <stingraykit/collection/ByteData.h>
#include <stingraykit/string/regex.h>

#include <algorithm>

#include <curl/curl.h>

namespace bikebrain
{

	static const int MaxDataEntriesSize = 3;

	std::string CurlErrorToString(CURLcode c);

#define CURL_CALL(...) \
	do { \
		CURLcode __res__ = (__VA_ARGS__); \
		STINGRAYKIT_CHECK(__res__ == CURLE_OK, stingray::Exception(#__VA_ARGS__ " failed: " + CurlErrorToString(__res__))); \
	} while (false)


	class CurlHolder
	{
	private:
		stingray::SharedHolder<CURL*>	_holder;

	public:
		CurlHolder()
			: _holder(&curl_easy_cleanup)
		{
			CURL* curl = curl_easy_init();
			STINGRAYKIT_CHECK(curl, "curl_easy_init() failed!");
			_holder.Set(curl);
		}

		operator CURL*() const	{ return Get(); }
		CURL* Get() const		{ return _holder.Get(); }
	};


	class HttpRequestBase
	{
	protected:
		static stingray::NamedLogger	s_logger;

		CurlHolder						_curl;
		struct curl_slist*				_headers;
		std::vector<uint8_t>			_responseData;

	public:
		HttpRequestBase(const std::string& url)
			: _headers(NULL)
		{
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_URL, url.c_str()));
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &HttpRequestBase::WriteCallback));
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this));
		}

		~HttpRequestBase()
		{
			if (_headers)
				curl_slist_free_all(_headers);
		}

		void AppendHeader(const std::string& header)
		{
			_headers = curl_slist_append(_headers, header.c_str());
			STINGRAYKIT_CHECK(_headers, "curl_slist_append failed!");
		}

		void Perform()
		{
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers));
			CURL_CALL(curl_easy_perform(_curl));

			s_logger.Info() << "Response: " << GetResponseCode() << ", Content Type: " << GetResponseContentType() << ", content: " << std::string(GetResponseData().begin(), GetResponseData().end());
		}

		int GetResponseCode() const
		{
			long responseCode = 0;
			CURL_CALL(curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &responseCode));
			return responseCode;
		}

		std::string GetResponseContentType() const
		{
			const char* contentType = NULL;
			CURL_CALL(curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &contentType));
			return contentType;
		}

		stingray::ConstByteData GetResponseData() const
		{ return stingray::ConstByteData(_responseData); }

	private:
		static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
		{
			HttpRequestBase* inst = static_cast<HttpRequestBase*>(userdata);
			return inst->WriteCallbackNonStatic(ptr, size, nmemb);
		}

		size_t WriteCallbackNonStatic(char *ptr, size_t size, size_t nmemb)
		{
			std::copy(ptr, ptr + size * nmemb, std::back_inserter(_responseData));
			return size * nmemb;
		}
	};

	STINGRAYKIT_DEFINE_NAMED_LOGGER(HttpRequestBase);


	class PostRequest : public HttpRequestBase
	{
	private:
		std::vector<uint8_t>	_data;
		size_t					_ofs;

	public:
		PostRequest(const std::string& url, const std::string& contentType, stingray::ConstByteData data)
			: HttpRequestBase(url), _ofs(0)
		{
			s_logger.Info() << "PostRequest(" << url << ", " << contentType << ", ...)";
			_data.resize(data.size());
			std::copy(data.begin(), data.end(), _data.begin());
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_POST, 1));
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, _data.data()));
			CURL_CALL(curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, _data.size()));
			AppendHeader("Content-Type: " + contentType);
			AppendHeader("Content-Length: " + stingray::ToString(_data.size()));
		}
	};


	class HttpTrip : public virtual ITrip
	{
	private:
		typedef std::vector<DataEntry>			DataEntriesVector;
	private:
		static stingray::NamedLogger			s_logger;

		int										_tripId;
		DataEntriesVector						_dataEntries;
		stingray::Mutex							_mutex;

	public:
		HttpTrip(const std::string& name)
			: _tripId(0)
		{
			std::string escapedName = stingray::regex_replace(name, stingray::regex("\""), "\\\"");
			std::string jsonStr = "{ \"trip\": { \"title\": \"" + escapedName + "\" } }";
			PostRequest r("bikebrains.herokuapp.com/trips.json", "application/json", stingray::ConstByteData((const uint8_t*)jsonStr.data(), jsonStr.size()));
			r.Perform();

			STINGRAYKIT_CHECK(r.GetResponseCode() / 100 == 2, stingray::StringBuilder() % "HTTP response code: " % r.GetResponseCode());

			std::string content(r.GetResponseData().begin(), r.GetResponseData().end());

			stingray::regex re(".*\"id\"\\s*:\\s*(\\d+).*");
			stingray::smatch m;
			stingray::regex_search(content, m, re);

			STINGRAYKIT_CHECK(m.size() == 2, "Could not parse server response: '" + content + "'!");
			_tripId = stingray::FromString<int>(m[1]);

			s_logger.Info() << "Created, tripId: " << _tripId;
		}

		~HttpTrip()
		{
			s_logger.Info() << "Destroying, tripId: " << _tripId;
			SendDataEntries();
		}

		virtual void ReportDataEntry(const DataEntry& dataEntry)
		{
			stingray::MutexLock l(_mutex);
			if (_dataEntries.size() >= MaxDataEntriesSize)
				SendDataEntries();
			_dataEntries.push_back(dataEntry);
		}

	private:
		void SendDataEntries()
		{
			s_logger.Info() << "SendDataEntries(): " << _dataEntries;

			STINGRAYKIT_SCOPE_EXIT(MK_PARAM(DataEntriesVector&, _dataEntries))
				_dataEntries.clear();
			STINGRAYKIT_SCOPE_EXIT_END;

			stingray::StringBuilder sb;
			sb % "{ \"route_points\": [ ";
			bool first = true;
			for (DataEntriesVector::const_iterator it = _dataEntries.begin(); it != _dataEntries.end(); ++it)
			{
				sb % (first ? "" : ", ") %
					"{ \"trip_id\": \"" % _tripId % "\", " %
					"\"gps_longitude\": \"" % it->GetGpsData().GetLongitude() % "\", " %
					"\"gps_latitude\": \"" % it->GetGpsData().GetLatitude() % "\", " %
					"\"gps_speed\": \"" % it->GetGpsData().GetSpeed() % "\", " %
					"\"gps_time\": \"" % it->GetGpsData().GetTime() % "\", " %
					"\"cadence\": \"" % it->GetCadence() % "\" }";

				first = false;
			}
			sb % " ] }";

			std::string jsonStr = sb.ToString();
			PostRequest r("bikebrains.herokuapp.com/route_points/multiple.json", "application/json", stingray::ConstByteData((const uint8_t*)jsonStr.data(), jsonStr.size()));
			r.Perform();

			STINGRAYKIT_CHECK(r.GetResponseCode() / 100 == 2, stingray::StringBuilder() % "HTTP response code: " % r.GetResponseCode());
		}
	};

	STINGRAYKIT_DEFINE_NAMED_LOGGER(HttpTrip);


	STINGRAYKIT_DEFINE_NAMED_LOGGER(HttpStatsEngine);

	HttpStatsEngine::HttpStatsEngine()
	{
		CURL_CALL(curl_global_init(CURL_GLOBAL_ALL));
		s_logger.Info() << "Created";
	}


	HttpStatsEngine::~HttpStatsEngine()
	{
		s_logger.Info() << "Destroying";
		curl_global_cleanup();
	}


	ITripPtr HttpStatsEngine::StartTrip(const std::string& name)
	{
		return stingray::make_shared<HttpTrip>(name);
	}


	////////////////////////////////////////////////////////////////////////////////


	std::string CurlErrorToString(CURLcode c)
	{
		switch (c)
		{
		case CURLE_OK: return "CURLE_OK";
		case CURLE_UNSUPPORTED_PROTOCOL: return "CURLE_UNSUPPORTED_PROTOCOL";
		case CURLE_FAILED_INIT: return "CURLE_FAILED_INIT";
		case CURLE_URL_MALFORMAT: return "CURLE_URL_MALFORMAT";
		case CURLE_NOT_BUILT_IN: return "CURLE_NOT_BUILT_IN";
		case CURLE_COULDNT_RESOLVE_PROXY: return "CURLE_COULDNT_RESOLVE_PROXY";
		case CURLE_COULDNT_RESOLVE_HOST: return "CURLE_COULDNT_RESOLVE_HOST";
		case CURLE_COULDNT_CONNECT: return "CURLE_COULDNT_CONNECT";
		case CURLE_FTP_WEIRD_SERVER_REPLY: return "CURLE_FTP_WEIRD_SERVER_REPLY";
		case CURLE_REMOTE_ACCESS_DENIED: return "CURLE_REMOTE_ACCESS_DENIED";
		case CURLE_FTP_ACCEPT_FAILED: return "CURLE_FTP_ACCEPT_FAILED";
		case CURLE_FTP_WEIRD_PASS_REPLY: return "CURLE_FTP_WEIRD_PASS_REPLY";
		case CURLE_FTP_ACCEPT_TIMEOUT: return "CURLE_FTP_ACCEPT_TIMEOUT";
		case CURLE_FTP_WEIRD_PASV_REPLY: return "CURLE_FTP_WEIRD_PASV_REPLY";
		case CURLE_FTP_WEIRD_227_FORMAT: return "CURLE_FTP_WEIRD_227_FORMAT";
		case CURLE_FTP_CANT_GET_HOST: return "CURLE_FTP_CANT_GET_HOST";
//		case CURLE_HTTP2: return "CURLE_HTTP2";
		case CURLE_FTP_COULDNT_SET_TYPE: return "CURLE_FTP_COULDNT_SET_TYPE";
		case CURLE_PARTIAL_FILE: return "CURLE_PARTIAL_FILE";
		case CURLE_FTP_COULDNT_RETR_FILE: return "CURLE_FTP_COULDNT_RETR_FILE";
		case CURLE_OBSOLETE20: return "CURLE_OBSOLETE20";
		case CURLE_QUOTE_ERROR: return "CURLE_QUOTE_ERROR";
		case CURLE_HTTP_RETURNED_ERROR: return "CURLE_HTTP_RETURNED_ERROR";
		case CURLE_WRITE_ERROR: return "CURLE_WRITE_ERROR";
		case CURLE_OBSOLETE24: return "CURLE_OBSOLETE24";
		case CURLE_UPLOAD_FAILED: return "CURLE_UPLOAD_FAILED";
		case CURLE_READ_ERROR: return "CURLE_READ_ERROR";
		case CURLE_OUT_OF_MEMORY: return "CURLE_OUT_OF_MEMORY";
		case CURLE_OPERATION_TIMEDOUT: return "CURLE_OPERATION_TIMEDOUT";
		case CURLE_OBSOLETE29: return "CURLE_OBSOLETE29";
		case CURLE_FTP_PORT_FAILED: return "CURLE_FTP_PORT_FAILED";
		case CURLE_FTP_COULDNT_USE_REST: return "CURLE_FTP_COULDNT_USE_REST";
		case CURLE_OBSOLETE32: return "CURLE_OBSOLETE32";
		case CURLE_RANGE_ERROR: return "CURLE_RANGE_ERROR";
		case CURLE_HTTP_POST_ERROR: return "CURLE_HTTP_POST_ERROR";
		case CURLE_SSL_CONNECT_ERROR: return "CURLE_SSL_CONNECT_ERROR";
		case CURLE_BAD_DOWNLOAD_RESUME: return "CURLE_BAD_DOWNLOAD_RESUME";
		case CURLE_FILE_COULDNT_READ_FILE: return "CURLE_FILE_COULDNT_READ_FILE";
		case CURLE_LDAP_CANNOT_BIND: return "CURLE_LDAP_CANNOT_BIND";
		case CURLE_LDAP_SEARCH_FAILED: return "CURLE_LDAP_SEARCH_FAILED";
		case CURLE_OBSOLETE40: return "CURLE_OBSOLETE40";
		case CURLE_FUNCTION_NOT_FOUND: return "CURLE_FUNCTION_NOT_FOUND";
		case CURLE_ABORTED_BY_CALLBACK: return "CURLE_ABORTED_BY_CALLBACK";
		case CURLE_BAD_FUNCTION_ARGUMENT: return "CURLE_BAD_FUNCTION_ARGUMENT";
		case CURLE_OBSOLETE44: return "CURLE_OBSOLETE44";
		case CURLE_INTERFACE_FAILED: return "CURLE_INTERFACE_FAILED";
		case CURLE_OBSOLETE46: return "CURLE_OBSOLETE46";
		case CURLE_TOO_MANY_REDIRECTS: return "CURLE_TOO_MANY_REDIRECTS";
		case CURLE_UNKNOWN_OPTION: return "CURLE_UNKNOWN_OPTION";
		case CURLE_TELNET_OPTION_SYNTAX: return "CURLE_TELNET_OPTION_SYNTAX";
		case CURLE_OBSOLETE50: return "CURLE_OBSOLETE50";
		case CURLE_PEER_FAILED_VERIFICATION: return "CURLE_PEER_FAILED_VERIFICATION";
		case CURLE_GOT_NOTHING: return "CURLE_GOT_NOTHING";
		case CURLE_SSL_ENGINE_NOTFOUND: return "CURLE_SSL_ENGINE_NOTFOUND";
		case CURLE_SSL_ENGINE_SETFAILED: return "CURLE_SSL_ENGINE_SETFAILED";
		case CURLE_SEND_ERROR: return "CURLE_SEND_ERROR";
		case CURLE_RECV_ERROR: return "CURLE_RECV_ERROR";
		case CURLE_OBSOLETE57: return "CURLE_OBSOLETE57";
		case CURLE_SSL_CERTPROBLEM: return "CURLE_SSL_CERTPROBLEM";
		case CURLE_SSL_CIPHER: return "CURLE_SSL_CIPHER";
		case CURLE_SSL_CACERT: return "CURLE_SSL_CACERT";
		case CURLE_BAD_CONTENT_ENCODING: return "CURLE_BAD_CONTENT_ENCODING";
		case CURLE_LDAP_INVALID_URL: return "CURLE_LDAP_INVALID_URL";
		case CURLE_FILESIZE_EXCEEDED: return "CURLE_FILESIZE_EXCEEDED";
		case CURLE_USE_SSL_FAILED: return "CURLE_USE_SSL_FAILED";
		case CURLE_SEND_FAIL_REWIND: return "CURLE_SEND_FAIL_REWIND";
		case CURLE_SSL_ENGINE_INITFAILED: return "CURLE_SSL_ENGINE_INITFAILED";
		case CURLE_LOGIN_DENIED: return "CURLE_LOGIN_DENIED";
		case CURLE_TFTP_NOTFOUND: return "CURLE_TFTP_NOTFOUND";
		case CURLE_TFTP_PERM: return "CURLE_TFTP_PERM";
		case CURLE_REMOTE_DISK_FULL: return "CURLE_REMOTE_DISK_FULL";
		case CURLE_TFTP_ILLEGAL: return "CURLE_TFTP_ILLEGAL";
		case CURLE_TFTP_UNKNOWNID: return "CURLE_TFTP_UNKNOWNID";
		case CURLE_REMOTE_FILE_EXISTS: return "CURLE_REMOTE_FILE_EXISTS";
		case CURLE_TFTP_NOSUCHUSER: return "CURLE_TFTP_NOSUCHUSER";
		case CURLE_CONV_FAILED: return "CURLE_CONV_FAILED";
		case CURLE_CONV_REQD: return "CURLE_CONV_REQD";
		case CURLE_SSL_CACERT_BADFILE: return "CURLE_SSL_CACERT_BADFILE";
		case CURLE_REMOTE_FILE_NOT_FOUND: return "CURLE_REMOTE_FILE_NOT_FOUND";
		case CURLE_SSH: return "CURLE_SSH";
		case CURLE_SSL_SHUTDOWN_FAILED: return "CURLE_SSL_SHUTDOWN_FAILED";
		case CURLE_AGAIN: return "CURLE_AGAIN";
		case CURLE_SSL_CRL_BADFILE: return "CURLE_SSL_CRL_BADFILE";
		case CURLE_SSL_ISSUER_ERROR: return "CURLE_SSL_ISSUER_ERROR";
		case CURLE_FTP_PRET_FAILED: return "CURLE_FTP_PRET_FAILED";
		case CURLE_RTSP_CSEQ_ERROR: return "CURLE_RTSP_CSEQ_ERROR";
		case CURLE_RTSP_SESSION_ERROR: return "CURLE_RTSP_SESSION_ERROR";
		case CURLE_FTP_BAD_FILE_LIST: return "CURLE_FTP_BAD_FILE_LIST";
		case CURLE_CHUNK_FAILED: return "CURLE_CHUNK_FAILED";
		case CURLE_NO_CONNECTION_AVAILABLE: return "CURLE_NO_CONNECTION_AVAILABLE";
		case CURL_LAST: return "CURL_LAST";
		default: return stingray::StringBuilder() % "Unknown curl error: " % c;
		}
	}

}
