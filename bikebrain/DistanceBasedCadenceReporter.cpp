#include <bikebrain/DistanceBasedCadenceReporter.h>

#include <stingraykit/optional.h>

#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR 1
#include <simple_fft/fft.h>


namespace bikebrain
{

	static const size_t MaxDataVectorSize = 512;
	static const stingray::TimeDuration PollSensorInterval = stingray::TimeDuration::FromMilliseconds(10);

	STINGRAYKIT_DEFINE_NAMED_LOGGER(DistanceBasedCadenceReporter);

	DistanceBasedCadenceReporter::DistanceBasedCadenceReporter(const IDistanceSensorPtr& distanceSensor)
		: _distanceSensor(distanceSensor), _timer(new stingray::Timer("cadenceReporter"))
	{
		_tokens += _timer->SetTimer(PollSensorInterval, stingray::bind(&DistanceBasedCadenceReporter::PollSensorFunc, this));
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
			STINGRAYKIT_SCOPE_EXIT(MK_PARAM(DataVector&, _data))
				_data.clear();
			STINGRAYKIT_SCOPE_EXIT_END;

			static const int windowSize = 16;

			double averageWavelength = 0;
			double wavelengthMeasurementsCount = 0;

			int direction = 0;
			stingray::optional<double> prevWindowAverage;
			stingray::optional<int> minIndex, maxIndex;
			for (int i = 0; i < (int)_data.size() - windowSize; ++i)
			{
				double windowAverage = 0;
				for (int j = 0; j < windowSize; ++j)
					windowAverage += _data[i + j];
				windowAverage /= windowSize;

				int newDirection = prevWindowAverage ? (*prevWindowAverage > windowAverage ? -1 : 1) : 0;
				prevWindowAverage = windowAverage;

				if (direction == -1 && newDirection == 1)
					maxIndex = i;
				else if (direction == 1 && newDirection == -1)
					minIndex = i;

				if (minIndex && maxIndex)
				{
					averageWavelength += (std::abs(*minIndex - *maxIndex) * 2) * (PollSensorInterval.GetMilliseconds() * 0.001);
					++wavelengthMeasurementsCount;
					minIndex.reset();
					maxIndex.reset();
				}

				direction = newDirection;
			}

			averageWavelength /= wavelengthMeasurementsCount;
			_cadence = 1 / averageWavelength;

			// TODO: implement fft-based cadence calculation
			//typedef std::complex<double>	ComplexD;
			//typedef std::vector<ComplexD>	ComplexDVector;

			//ComplexDVector fftOut(_data.size());

			//const char* errorDescription = NULL;
			//STINGRAYKIT_CHECK(simple_fft::FFT(_data, fftOut, _data.size(), errorDescription), stingray::Exception(stingray::StringBuilder() % "simple_fft::FFT failed: " % (errorDescription ? errorDescription : "Unknown error")));

			//for (int i = 0; i < fftOut.size(); ++i)
				//s_logger.Info() << i << ": " << fftOut[i].real();
		}

		_data.push_back(_distanceSensor->GetDistance());
	}

}
