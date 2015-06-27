#ifndef BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H
#define BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H

#include <bikebrain/ICadenceReporter.h>
#include <bikebrain/IDistanceSensor.h>

#include <stingraykit/log/Logger.h>
#include <stingraykit/signal/signals.h>
#include <stingraykit/timer/Timer.h>

namespace bikebrain
{

	class DistanceBasedCadenceReporter : public virtual ICadenceReporter
	{
	private:
		static stingray::NamedLogger		s_logger;

		IDistanceSensorPtr					_distanceSensor;
		stingray::TimerPtr					_timer;

		stingray::TokenPool					_tokens;

	public:
		DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor);
		~DistanceBasedCadenceReporter();

		virtual double GetCadence() const { return 0; }

	private:
		void PollSensorFunc();
	};

}

#endif
