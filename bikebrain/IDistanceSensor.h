#ifndef BIKEBRAIN_IDISTANCESENSOR_H
#define BIKEBRAIN_IDISTANCESENSOR_H

namespace bikebrain
{

	struct IDistanceSensor
	{
		virtual ~IDistanceSensor() { }

		virtual double GetDistance() const = 0; // Distance in meters
	};
	STINGRAYKIT_DECLARE_PTR(IDistanceSensor);

}

#endif
