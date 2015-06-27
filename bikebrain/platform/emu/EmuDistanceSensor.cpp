#include <bikebrain/platform/emu/EmuDistanceSensor.h>

#include <math.h>


namespace bikebrain {
namespace emu
{

	EmuDistanceSensor::EmuDistanceSensor(double amplitude, double frequency)
		: _amplitude(amplitude), _frequency(frequency)
	{ }


	double EmuDistanceSensor::GetDistance() const
	{ return _amplitude * sin(_frequency * (_elapsedTime.ElapsedMicroseconds() / 1000000.0)); }

}}
