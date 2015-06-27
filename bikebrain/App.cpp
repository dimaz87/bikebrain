#include <bikebrain/App.h>


namespace bikebrain
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(App);

	App::App()
		: _worker(stingray::ITaskExecutor::Create("app"))
	{
#ifdef PLATFORM_EMU
		ILedMatrixPtr ledMatrix = stingray::make_shared<emu::EmuLedMatrix>();
		// ...
#endif

		_display->SetBacklightColor(RGB(255, 255, 255));

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
	}


	void App::TurnIndicatorStateChangedHandler(TurnIndicatorState state)
	{
		s_logger.Info() << "TurnIndicatorStateChangedHandler(" << state << ")";
	}

}
