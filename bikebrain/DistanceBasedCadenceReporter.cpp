#include <bikebrain/DistanceBasedCadenceReporter.h>


namespace bikebrain
{

	static const size_t MaxDataVectorSize = 500;

	STINGRAYKIT_DEFINE_NAMED_LOGGER(DistanceBasedCadenceReporter);

	DistanceBasedCadenceReporter::DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor)
		: _distanceSensor(distanceSensor), _timer(new stingray::Timer("cadenceReporter"))
	{
		_tokens += _timer->SetTimer(stingray::TimeDuration::FromMilliseconds(10), stingray::bind(&DistanceBasedCadenceReporter::PollSensorFunc, this));
		s_logger.Info() << "Created";
	}


	DistanceBasedCadenceReporter::~DistanceBasedCadenceReporter()
	{
		s_logger.Info() << "Destroying";
		_tokens.Release();
	}


	void DistanceBasedCadenceReporter::PollSensorFunc()
	{
		if (_data.size() >= MaxDataVectorSize)
		{
			double average = 0;
			for (DataVector::const_iterator it = _data.begin(); it != _data.end(); ++it)
				average += *it;
			average /= _data.size();
			_cadence = average;
			s_logger.Info() << "Average distance value: " << average;
			_data.clear();
		}

		_data.push_back(_distanceSensor->GetDistance());
	}

}
