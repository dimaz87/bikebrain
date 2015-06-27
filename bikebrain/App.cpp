#include <bikebrain/App.h>


namespace bikebrain
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(App);

	App::App()
		: _worker(stingray::ITaskExecutor::Create("app"))
	{
#ifdef PLATFORM_EMU
		ILedMatrixPtr ledMatrix = stingray::make_shared<emu::EmuLedMatrix>();
#endif
		s_logger.Info() << "Created";
	}


	App::~App()
	{
		s_logger.Info() << "Destroying";
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
