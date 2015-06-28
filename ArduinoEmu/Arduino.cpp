#include <Arduino.h>

#include <bikebrain/platform/edison/MraaCall.h>

#include <stingraykit/exception.h>
#include <stingraykit/shared_ptr.h>
#include <stingraykit/log/Logger.h>

#include <map>

#include <mraa.hpp>

using mraa::Gpio;
STINGRAYKIT_DECLARE_PTR(Gpio);

typedef std::map<int, GpioPtr>	GpioCache;
static GpioCache g_gpioCache;

static GpioPtr GetGpio(int pin)
{
	GpioCache::const_iterator it = g_gpioCache.find(pin);
	if (it == g_gpioCache.end())
	{
		stingray::Logger::Info() << "Created GPIO pin " << pin;
		it = g_gpioCache.insert(std::make_pair(pin, stingray::make_shared<Gpio>(pin))).first;
	}
	return it->second;
}

void digitalWrite(int pin, bool value)
{
	MRAA_CALL(GetGpio(pin)->write(value));
}

void pinMode(int pin, int mode)
{
	STINGRAYKIT_CHECK(mode == OUTPUT, stingray::ArgumentException("mode"));
	MRAA_CALL(GetGpio(pin)->dir(mraa::DIR_OUT));
}
