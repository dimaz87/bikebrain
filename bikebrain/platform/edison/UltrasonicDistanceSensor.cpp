#include <bikebrain/platform/edison/UltrasonicDistanceSensor.h>

#include <stingraykit/thread/Thread.h>
#include <stingraykit/time/ElapsedTime.h>

namespace bikebrain {
namespace edison
{

	UltrasonicDistanceSensor::UltrasonicDistanceSensor(int pin)
		: _pwm(pin)
	{ }


	double UltrasonicDistanceSensor::DistanceMeasure() const
	{
		_pwm.pulsewidth_us(5);

		int us = 0;
		stingray::ElapsedTime elapsed;
		while (_pwm.read() > 0.7f && us < 100000)
			us = elapsed.ElapsedMicroseconds();

		return us / 58000.0;
	}


	double UltrasonicDistanceSensor::GetDistance() const
	{
		return DistanceMeasure();
	}

}}
