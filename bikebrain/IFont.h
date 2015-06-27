#ifndef BIKEBRAIN_IFONT_H
#define BIKEBRAIN_IFONT_H

#include <bikebrain/ILedMatrix.h>

#include <stingraykit/Rect.h>

namespace bikebrain
{

	struct IFont
	{
		virtual ~IFont() { }

		virtual stingray::Size GetSymbolSize() const = 0;
		virtual void DrawLetter(const ILedMatrixPtr& ledMatrix, int x, int y, char c) const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IFont);

}

#endif
