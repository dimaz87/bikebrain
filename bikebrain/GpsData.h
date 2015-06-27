#ifndef BIKEBRAIN_GPSDATA_H
#define BIKEBRAIN_GPSDATA_H

#include <stingraykit/time/Time.h>

namespace bikebrain
{

	class GpsData
	{
	private:
		double				_longitude;
		double				_latitude;
		double				_speed;
		stingray::Time		_time;

	public:
		GpsData(double longitude, double latitude, double speed, stingray::Time time)
			: _longitude(longitude), _latitude(latitude), _speed(speed), _time(time)
		{ }

		double GetLongitude() const		{ return _longitude; }
		double GetLatitude() const		{ return _latitude; }
		double GetSpeed() const			{ return _speed; }
		stingray::Time GetTime() const	{ return _time; }
	};

}

#endif
