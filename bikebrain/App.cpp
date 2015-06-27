#include <bikebrain/App.h>
#include <bikebrain/DistanceBasedCadenceReporter.h>
#include <bikebrain/WrappedCFont.h>

#ifdef PLATFORM_EMU
#	include <bikebrain/platform/emu/EmuButton.h>
#	include <bikebrain/platform/emu/EmuDisplay.h>
#	include <bikebrain/platform/emu/EmuTextDisplay.h>
#	include <bikebrain/platform/emu/EmuDistanceSensor.h>
#endif

namespace bikebrain
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(App);

	App::App()
		: _timer(new stingray::Timer("app"))
	{
#ifdef PLATFORM_EMU
		_ledMatrix			= stingray::make_shared<emu::EmuDisplay>("turnIndicator", stingray::Size(32, 16));
		_distanceSensor		= stingray::make_shared<emu::EmuDistanceSensor>(30, 2);
		_textDisplay		= stingray::make_shared<emu::EmuTextDisplay>();

		emu::StdinReaderPtr stdinReader = stingray::make_shared<emu::StdinReader>();
		_leftButton			= stingray::make_shared<emu::EmuButton>(stdinReader, "left");
		_rightButton		= stingray::make_shared<emu::EmuButton>(stdinReader, "right");
#endif
		_cadenceReporter	= stingray::make_shared<DistanceBasedCadenceReporter>(_distanceSensor);
		_font				= stingray::make_shared<WrappedCFont>();

		_textDisplay->SetBacklightColor(RGB(255, 255, 255));

		_tokens += _timer->SetTimer(stingray::TimeDuration::FromSeconds(3), stingray::bind(&App::PollDataFunc, this));
		_tokens += _leftButton->OnPressed().connect(_timer, stingray::bind(&App::ButtonPressedHandler, this, "Left"));
		_tokens += _rightButton->OnPressed().connect(_timer, stingray::bind(&App::ButtonPressedHandler, this, "Right"));

		_tokens += _turnIndicatorState.OnChanged().connect(_timer, stingray::bind(&App::TurnIndicatorStateChangedHandler, this, stingray::_1));

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


	void App::TurnIndicatorStateChangedHandler(TurnIndicatorState state)
	{
		s_logger.Info() << "TurnIndicatorStateChangedHandler(" << state << ")";
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
	}


	void App::PollDataFunc()
	{
		s_logger.Info() << "PollDataFunc()";
		_textDisplay->SetText(stingray::StringBuilder() % "cad: " % _cadenceReporter->GetCadence());
	}



}
