#include <bikebrain/App.h>

#include <bikebrain/DistanceBasedCadenceReporter.h>
#include <bikebrain/WrappedCFont.h>

#ifdef PLATFORM_EMU
#	include <bikebrain/platform/emu/EmuDisplay.h>
#	include <bikebrain/platform/emu/EmuDistanceSensor.h>
#endif

namespace bikebrain
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(App);

	App::App()
		: _worker(stingray::ITaskExecutor::Create("app"))
	{
#ifdef PLATFORM_EMU
		_ledMatrix			= stingray::make_shared<emu::EmuDisplay>("turnIndicator", stingray::Size(32, 16));
		_distanceSensor		= stingray::make_shared<emu::EmuDistanceSensor>
		// ...
#endif
		_cadenceReporter	= stingray::make_shared<DistanceBasedCadenceReporter>(_distanceSensor);
		_font				= stingray::make_shared<WrappedCFont>();

		_textDisplay->SetBacklightColor(RGB(255, 255, 255));

		_tokens += _cadenceReporter->OnCadence().connect(_worker, stingray::bind(&App::CadenceChangedHandler, this, stingray::_1));
		_tokens += _leftButton->OnPressed().connect(_worker, stingray::bind(&App::ButtonPressedHandler, this, "Left"));
		_tokens += _rightButton->OnPressed().connect(_worker, stingray::bind(&App::ButtonPressedHandler, this, "Right"));

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
	}


	void App::CadenceChangedHandler(double cadence)
	{
		s_logger.Info() << "CadenceChangedHandler(" << cadence << ")";
		_textDisplay->SetText(stingray::StringBuilder() % "cad: " % cadence);
	}


	void App::TurnIndicatorStateChangedHandler(TurnIndicatorState state)
	{
		s_logger.Info() << "TurnIndicatorStateChangedHandler(" << state << ")";
		switch (state)
		{
		case TurnIndicatorState::None:
			_font->DrawString(_ledMatrix, 0, 0, "   ");
			break;
		case TurnIndicatorState::Left:
			_font->DrawString(_ledMatrix, 0, 0, "<<<");
			break;
		case TurnIndicatorState::Right:
			_font->DrawString(_ledMatrix, 0, 0, ">>>");
			break;
		}
	}

}
