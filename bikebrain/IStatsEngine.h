#ifndef BIKEBRAIN_ISTATSENGINE_H
#define BIKEBRAIN_ISTATSENGINE_H

#include <bikebrain/DataEntry.h>

namespace bikebrain
{

	struct ITrip
	{
		virtual ~ITrip() { }

		virtual void ReportDataEntry(const DataEntry& dataEntry) = 0;
	};
	STINGRAYKIT_DECLARE_PTR(ITrip);


	struct IStatsEngine
	{
		virtual ~IStatsEngine() { }

		virtual ITripPtr StartTrip(const std::string& name) = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IStatsEngine);

}

#endif
