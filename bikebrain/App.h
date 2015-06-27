#ifndef BIKEBRAIN_APP_H
#define BIKEBRAIN_APP_H

#include <bikebrain/IButton.h>
#include <bikebrain/ICadenceReporter.h>
#include <bikebrain/IDisplay.h>
#include <bikebrain/ILedMatrix.h>

#include <stingraykit/toolkit.h>

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
		ITextDisplayPtr									_textDisplay;
		IFontPtr										_font;

		stingray::ITaskExecutorPtr						_worker;

		stingray::TokenPool								_tokens;

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
