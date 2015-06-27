#ifndef BIKEBRAIN_ILEDMATRIX_H
#define BIKEBRAIN_ILEDMATRIX_H

#include <stingraykit/Rect.h>

namespace bikebrain
{

	struct ILedMatrix
	{
		virtual ~ILedMatrix() { }

		virtual stingray::Size GetResolution() const = 0;
		virtual void EnableLed(int i, int j, bool enable) = 0;
	};
	STINGRAYKIT_DECLARE_PTR(ILedMatrix);

}

#endif
