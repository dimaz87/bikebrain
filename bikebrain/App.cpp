#include <bikebrain/App.h>

#include <bikebrain/DistanceBasedCadenceReporter.h>
#include <bikebrain/HttpStatsEngine.h>
#include <bikebrain/WrappedCFont.h>
#include <bikebrain/platform/emu/EmuStatsEngine.h>

#ifdef PLATFORM_EMU
#	include <bikebrain/platform/emu/EmuButton.h>
#	include <bikebrain/platform/emu/EmuDisplay.h>
#	include <bikebrain/platform/emu/EmuDistanceSensor.h>
#	include <bikebrain/platform/emu/EmuGpsModule.h>
#	include <bikebrain/platform/emu/EmuTextDisplay.h>
#elif defined(PLATFORM_EDISON)
#	include <bikebrain/platform/edison/Button.h>
#endif

#define USE_ANIMATED_TURN_INDICATOR !defined(PLATFORM_EMU)

namespace bikebrain
{

	static const stingray::TimeDuration UpdateTurnIndicatorInterval = stingray::TimeDuration::FromMilliseconds(60);

	STINGRAYKIT_DEFINE_NAMED_LOGGER(App);

	App::App()
		: _timer(new stingray::Timer("app")), _dataReportWorker(stingray::ITaskExecutor::Create("dataReporter"))
	{
#ifdef PLATFORM_EMU
		_gpsModule			= stingray::make_shared<emu::EmuGpsModule>();
		_ledMatrix			= stingray::make_shared<emu::EmuDisplay>("turnIndicator", stingray::Size(32, 16));
		_distanceSensor		= stingray::make_shared<emu::EmuDistanceSensor>(30, 2);
		_textDisplay		= stingray::make_shared<emu::EmuTextDisplay>();

		emu::StdinReaderPtr stdinReader = stingray::make_shared<emu::StdinReader>();
		_leftButton			= stingray::make_shared<emu::EmuButton>(stdinReader, "left");
		_rightButton		= stingray::make_shared<emu::EmuButton>(stdinReader, "right");
		_controlButton		= stingray::make_shared<emu::EmuButton>(stdinReader, "control");
#elif defined(PLATFORM_EDISON)
		_controlButton		= stingray::make_shared<edison::Button>(0);
#endif

		_statsEngine		= stingray::make_shared<HttpStatsEngine>();
		_cadenceReporter	= stingray::make_shared<DistanceBasedCadenceReporter>(_distanceSensor);
		_font				= stingray::make_shared<WrappedCFont>();

		_textDisplay->SetBacklightColor(RGB(255, 255, 255));

		_tokens += _timer->SetTimer(stingray::TimeDuration::FromSeconds(3), stingray::bind(&App::PollDataFunc, this));
		_tokens += _leftButton->OnPressed().connect(_timer, stingray::bind(&App::ButtonPressedHandler, this, "Left"));
		_tokens += _rightButton->OnPressed().connect(_timer, stingray::bind(&App::ButtonPressedHandler, this, "Right"));
		_tokens += _controlButton->OnPressed().connect(_timer, stingray::bind(&App::ButtonPressedHandler, this, "Control"));

		_tokens += _turnIndicatorState.OnChanged().connect(_timer, stingray::bind(&App::TurnIndicatorStateChangedHandler, this, stingray::_1));
		_tokens += _activeTripState.OnChanged().connect(_timer, stingray::bind(&App::ActiveTripStateChangedHandler, this, stingray::_1));

#if USE_ANIMATED_TURN_INDICATOR
		_tokens += _timer->SetTimer(UpdateTurnIndicatorInterval, stingray::bind(&App::UpdateTurnIndicator, this));
#endif

		s_logger.Info() << "Created";
	}


	App::~App()
	{
		s_logger.Info() << "Destroying";
		_tokens.Release();
	}


	void App::Run()
	{
		while (true)
			stingray::Thread::Sleep(1000);
	}


	void App::ButtonPressedHandler(const std::string& button)
	{
		s_logger.Info() << "ButtonPressedHandler(" << button << ")";
		if (button == "Left")
			_turnIndicatorState = _turnIndicatorState.Get() == TurnIndicatorState::Left ? TurnIndicatorState::None : TurnIndicatorState::Left;
		else if (button == "Right")
			_turnIndicatorState = _turnIndicatorState.Get() == TurnIndicatorState::Right ? TurnIndicatorState::None : TurnIndicatorState::Right;
		else if (button == "Control")
			_activeTripState = !_activeTripState.Get();
	}


	void App::TurnIndicatorStateChangedHandler(TurnIndicatorState state)
	{
		s_logger.Info() << "TurnIndicatorStateChangedHandler(" << state << ")";
#if !USE_ANIMATED_TURN_INDICATOR
		switch (state)
		{
		case TurnIndicatorState::None:
			_font->DrawString(_ledMatrix, 0, 0, "    ");
			break;
		case TurnIndicatorState::Left:
			_font->DrawString(_ledMatrix, 0, 0, "LEFT");
			break;
		case TurnIndicatorState::Right:
			_font->DrawString(_ledMatrix, 0, 0, "RGHT");
			break;
		}
#endif
	}


	void App::ActiveTripStateChangedHandler(bool state)
	{
		_dataReportWorker->AddTask(stingray::bind(state ? &App::DoStartTrip : &App::DoStopTrip, this));
	}


	void App::UpdateTurnIndicator()
	{
		int direction = 0;
		std::string s;
		switch (_turnIndicatorState.Get())
		{
		case TurnIndicatorState::None:
			s = "     ";
			break;
		case TurnIndicatorState::Left:
			s = "< < < <";
			direction = -1;
			break;
		case TurnIndicatorState::Right:
			s = "> > > > >";
			direction = 1;
			break;
		}
		_font->DrawString(_ledMatrix, (direction * _elapsedTime.ElapsedMilliseconds() / UpdateTurnIndicatorInterval.GetMilliseconds()) % 16 - 16, 4, s);
	}


	void App::PollDataFunc()
	{
		s_logger.Info() << "PollDataFunc()";

		double cadence = _cadenceReporter->GetCadence();
		GpsData gpsData = _gpsModule->GetData();

		_textDisplay->SetText(stingray::StringBuilder() % "cad: " % cadence % "\ntrip: " % _activeTripState.Get());

		_dataReportWorker->AddTask(stingray::bind(&App::DoReportData, this, DataEntry(gpsData, cadence)));
	}


	void App::DoStartTrip()
	{ _trip = _statsEngine->StartTrip(stingray::StringBuilder() % "Trip " % stingray::Time::Now()); }


	void App::DoStopTrip()
	{ _trip.reset(); }


	void App::DoReportData(const DataEntry& dataEntry)
	{
		if (_trip)
			_trip->ReportDataEntry(dataEntry);
	}

}
