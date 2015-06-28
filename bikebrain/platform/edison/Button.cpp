#include <bikebrain/platform/edison/Button.h>

#include <stingraykit/log/Logger.h>

using namespace stingray;

namespace bikebrain {
namespace edison
{

	Button::Button(int portNumber) : _gpio(portNumber, Gpio::Edge::Both), _pressed(false)
	{
		_connection = _gpio.OnEvent().connect(bind(&Button::GpioEventHandler, this, stingray::_1));
	}

	void Button::GpioEventHandler(bool gpioLevel)
	{
		if (gpioLevel)
		{
			_elapsed.Restart();
			_pressed = true;
		}
		else if (_pressed)
		{
			_onPressed(_elapsed.ElapsedMilliseconds());
			_pressed = false;
		}
	}

}}
