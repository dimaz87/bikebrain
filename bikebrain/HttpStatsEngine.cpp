#include <bikebrain/HttpStatsEngine.h>


namespace bikebrain
{

	class HttpTrip : public virtual ITrip
	{
	private:
		static stingray::NamedLogger			s_logger;

	public:
		HttpTrip()
		{
			s_logger.Info() << "Created";
		}

		~HttpTrip()
		{
			s_logger.Info() << "Destroying";
		}

		virtual void ReportDataEntry(const DataEntry& dataEntry)
		{
		}
	};

	STINGRAYKIT_DEFINE_NAMED_LOGGER(HttpTrip);


	STINGRAYKIT_DEFINE_NAMED_LOGGER(HttpStatsEngine);

	HttpStatsEngine::HttpStatsEngine()
	{
		s_logger.Info() << "Created";
	}


	HttpStatsEngine::~HttpStatsEngine()
	{
		s_logger.Info() << "Destroying";
	}


	ITripPtr HttpStatsEngine::StartTrip()
	{
		return stingray::make_shared<HttpTrip>();
	}

}
