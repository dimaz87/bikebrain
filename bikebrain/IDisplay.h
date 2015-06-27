#ifndef BIKEBRAIN_IDISPLAY_H
#define BIKEBRAIN_IDISPLAY_H

#include <bikebrain/ILedMatrix.h>
#include <bikebrain/RGB.h>

#include <stingraykit/Rect.h>

namespace bikebrain
{

	struct IDisplay : public virtual ILedMatrix
	{
		virtual ~IDisplay() { }

		virtual void SetBacklightColor(RGB color) const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IDisplay);

}

#endif
