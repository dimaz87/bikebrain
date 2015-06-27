#include <bikebrain/platform/emu/EmuDistanceSensor.h>

#include <math.h>


namespace bikebrain {
namespace emu
{

	EmuDistanceSensor::EmuDistanceSensor(double amplitude, double frequency)
		: _amplitude(amplitude), _frequency(frequency)
	{ }


	double EmuDistanceSensor::GetDistance() const
	{ return _amplitude * sin(2 * M_PI * _frequency * (_elapsedTime.ElapsedMicroseconds() / 1000000.0)); }

}}
