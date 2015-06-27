#ifndef BIKEBRAIN_HTTPSTATSENGINE_H
#define BIKEBRAIN_HTTPSTATSENGINE_H

#include <bikebrain/IStatsEngine.h>

#include <stingraykit/log/Logger.h>

namespace bikebrain
{

	class HttpStatsEngine : public virtual IStatsEngine
	{
	private:
		static stingray::NamedLogger				s_logger;

	public:
		HttpStatsEngine();
		~HttpStatsEngine();

		virtual ITripPtr StartTrip(const std::string& name);
	};

}

#endif
