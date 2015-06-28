#ifndef BIKEBRAIN_PLATFORM_EDISON_BUTTON_H
#define BIKEBRAIN_PLATFORM_EDISON_BUTTON_H

#include <bikebrain/IButton.h>
#include <bikebrain/platform/edison/Gpio.h>

#include <stingraykit/exception.h>
#include <stingraykit/Token.h>
#include <stingraykit/signal/signals.h>
#include <stingraykit/time/ElapsedTime.h>

namespace bikebrain {
namespace edison
{

	class Button : public virtual IButton
	{
	private:
		Gpio									_gpio;
		bool									_pressed;
		stingray::ElapsedTime					_elapsed;
		stingray::signal<void (stingray::u32)>	_onPressed;
		stingray::TokenHolder					_connection;

	public:
		Button(int portNumber);

		virtual stingray::signal_connector<void (stingray::u32)> OnPressed() const { return _onPressed.connector(); }

	private:
		void GpioEventHandler(bool gpioLevel);
	};

}}

#endif
