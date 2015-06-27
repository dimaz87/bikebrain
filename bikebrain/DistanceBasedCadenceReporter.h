#ifndef BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H
#define BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H

#include <bikebrain/ICadenceReporter.h>
#include <bikebrain/IDistanceSensor.h>

namespace bikebrain
{

	class DistanceBasedCadenceReporter : public virtual ICadenceReporter
	{
	private:
		static stingray::NamedLogger		s_logger;

		IDistanceSensorPtr					_distanceSensor;
		stingray::signal<void(double)>		_onCadence;
		stingray::TimerPtr					_timer;

		stingray::TokenPool					_tokens;

	public:
		DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor);
		~DistanceBasedCadenceReporter();

		virtual stingray::signal_connector<void(double)> OnCadence() const
		{ return _onCadence; }

	private:
		void PollSensorFunc();
	};

}

#endif
