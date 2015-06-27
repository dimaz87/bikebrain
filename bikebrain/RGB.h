#ifndef BIKEBRAIN_RGB_H
#define BIKEBRAIN_RGB_H

#include <stingraykit/string/ToString.h>

#include <stdint.h>

namespace bikebrain
{

	struct RGB
	{
		uint8_t		R;
		uint8_t		G;
		uint8_t		B;

		RGB()
			: R(0), G(0), B(0)
		{ }

		RGB(uint8_t r, uint8_t g, uint8_t b)
			: R(r), G(g), B(b)
		{ }

		std::string ToString() const
		{ return stingray::StringBuilder() % "(" % R % ", " % G % ", " % B % ")"; }
	};

}

#endif
