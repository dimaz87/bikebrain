#ifndef BIKEBRAIN_IDISPLAY_H
#define BIKEBRAIN_IDISPLAY_H

#include <bikebrain/ILedMatrix.h>

#include <stingraykit/Rect.h>

#include <stdint.h>

namespace bikebrain
{

	struct RGB
	{
		uint8_t		R;
		uint8_t		G
		uint8_t		B;

		RGB()
			: R(0), G(0), B(0)
		{ }

		RGB(uint8_t r, uint8_t g, uint8_t b)
			: R(r), G(g), B(b)
		{ }
	};


	struct IDisplay : public virtual ILedMatrix
	{
		virtual ~IDisplay() { }

		virtual void SetBacklightColor(RGB color) const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IDisplay);

}

#endif
