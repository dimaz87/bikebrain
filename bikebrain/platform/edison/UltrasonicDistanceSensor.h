#ifndef BIKEBRAIN_PLATFORM_EDISON_ULTRASONICDISTANCESENSOR_H
#define BIKEBRAIN_PLATFORM_EDISON_ULTRASONICDISTANCESENSOR_H

#include <bikebrain/IDistanceSensor.h>

#include <stingraykit/ObservableValue.h>

#include "mraa.hpp"

namespace bikebrain {
namespace edison
{

	class UltrasonicDistanceSensor : public virtual IDistanceSensor
	{
	private:
		mutable mraa::Pwm		_pwm;

	public:
		UltrasonicDistanceSensor(int pin);

		virtual double GetDistance() const;

	private:
		double DistanceMeasure(void) const;
	};

}}

#endif
