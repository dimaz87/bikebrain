#ifndef BIKEBRAIN_APP_H
#define BIKEBRAIN_APP_H

namespace bikebrain
{

	class App
	{
		struct TurnIndicatorState
		{
			STINGRAYKIT_ENUM_VALUES(None, Left, Right);
			STINGRAYKIT_DECLARE_ENUM_CLASS(TurnIndicatorState);
		};

	private:
		static stingray::NamedLogger					s_logger;

		stingray::ObservableValue<TurnIndicatorState>	_turnIndicatorState;

		IButtonPtr										_leftButton;
		IButtonPtr										_rightButton;
		ILedMatrixPtr									_ledMatrix;
		ICadenceReporterPtr								_cadenceReporter;

		stingray::ITaskExecutorPtr						_worker;

	public:
		App();
		~App();

		void Run();

	private:
		void ButtonPressedHandler(const std::string& button);
		void CadenceChangedHandler(double cadence);

		void TurnIndicatorStateChangedHandler(TurnIndicatorState state);
	};

}

#endif
