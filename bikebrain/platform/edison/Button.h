#ifndef BIKEBRAIN_PLATFORM_EDISON_BUTTON_H
#define BIKEBRAIN_PLATFORM_EDISON_BUTTON_H

#include <bikebrain/IButton.h>
#include <bikebrain/platform/edison/Gpio.h>

#include <stingraykit/signal/signals.h>

namespace bikebrain {
namespace edison
{

	class Button : public virtual IButton
	{
	private:
		Gpio						_gpio;
		stingray::signal<void ()>	_onPressed;

	public:
		Button(int portNumber);

		virtual stingray::signal_connector<void ()> OnPressed() const { return _onPressed.connector(); }

	private:
	};

}}

#endif
