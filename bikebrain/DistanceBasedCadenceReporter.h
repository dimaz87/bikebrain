#ifndef BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H
#define BIKEBRAIN_DISTANCEBASEDCADENCEREPORTER_H

#include <bikebrain/ICadenceReporter.h>
#include <bikebrain/IDistanceSensor.h>

#include <stingraykit/ObservableValue.h>
#include <stingraykit/log/Logger.h>
#include <stingraykit/signal/signals.h>
#include <stingraykit/timer/Timer.h>

namespace bikebrain
{

	class DistanceBasedCadenceReporter : public virtual ICadenceReporter
	{
	private:
		typedef std::vector<double>			DataVector;

	private:
		static stingray::NamedLogger		s_logger;

		DataVector							_data;
		stingray::ObservableValue<double>	_cadence;
		IDistanceSensorPtr					_distanceSensor;
		stingray::TimerPtr					_timer;

		stingray::TokenPool					_tokens;

	public:
		DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor);
		~DistanceBasedCadenceReporter();

		virtual double GetCadence() const { return _cadence.Get(); }

	private:
		void PollSensorFunc();
	};

}

#endif
