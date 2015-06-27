#include <bikebrain/platform/edison/Button.h>

#include <stingraykit/exception.h>

#include <mraa.hpp>

using namespace stingray;

namespace bikebrain {
namespace edison
{

	class Button::ButtonImpl
	{
	private:
		shared_ptr<mraa::Gpio>	_gpio;

	public:
		ButtonImpl() : _gpio(new mraa::Gpio(128, false, false))
		{

		}
	};


	Button::Button()
	{
		STINGRAYKIT_THROW(NotImplementedException());
	}


	Button::~Button()
	{

	}


	stingray::signal_connector<void()> Button::OnPressed() const
	{
		STINGRAYKIT_THROW(NotImplementedException());
	}

}}
