#include <bikebrain/platform/edison/Button.h>

#include <stingraykit/exception.h>

using namespace stingray;

namespace bikebrain {
namespace edison
{

	Button::Button(int portNumber) : _gpio(portNumber, Gpio::Edge::Rising) // TODO: change to Edge::Both to implement hold detection
	{

	}

}}
