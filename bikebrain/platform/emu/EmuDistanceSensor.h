#ifndef BIKEBRAIN_PLATFORM_EMUDISTANCESENSOR_H
#define BIKEBRAIN_PLATFORM_EMUDISTANCESENSOR_H

#include <bikebrain/IDistanceSensor.h>

#include <stingraykit/time/ElapsedTime.h>

namespace bikebrain {
namespace emu
{

	class EmuDistanceSensor : public virtual IDistanceSensor
	{
	private:
		stingray::ElapsedTime		_elapsedTime;
		double						_amplitude;
		double						_frequency;

	public:
		EmuDistanceSensor(double amplitude, double frequency);

		virtual double GetDistance() const;
	};

}}

#endif
