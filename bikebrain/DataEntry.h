#ifndef BIKEBRAIN_DATAENTRY_H
#define BIKEBRAIN_DATAENTRY_H

#include <bikebrain/GpsData.h>

#include <stingraykit/string/ToString.h>

namespace bikebrain
{

	class DataEntry
	{
	private:
		GpsData		_gpsData;
		double		_cadence;

	public:
		DataEntry(GpsData gpsData, double cadence)
			: _gpsData(gpsData), _cadence(cadence)
		{ }

		GpsData GetGpsData() const	{ return _gpsData; }
		double GetCadence() const	{ return _cadence; }

		std::string ToString() const
		{ return stingray::StringBuilder() % "{ cadence: " % _cadence % " }"; }
	};

}

#endif
