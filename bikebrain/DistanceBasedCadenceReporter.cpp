#include <bikebrain/DistanceBasedCadenceReporter.h>


namespace bikebrain
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(DistanceBasedCadenceReporter);

	DistanceBasedCadenceReporter::DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor)
		: _distanceSensor(distanceSensor), _timer(new stingray::Timer("cadenceReporter"))
	{
		_tokens += _timer->SetTimer(stingray::TimeDuration::FromSeconds(1), stingray::bind(&DistanceBasedCadenceReporter::PollSensorFunc, this));
		s_logger.Info() << "Created";
	}


	DistanceBasedCadenceReporter::~DistanceBasedCadenceReporter()
	{
		s_logger.Info() << "Destroying";
		_tokens.Release();
	}


	void DistanceBasedCadenceReporter::PollSensorFunc()
	{
		s_logger.Info() << "Distance: " << _distanceSensor->GetDistance();
		_onCadence(0);
	}

}
