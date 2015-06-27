#ifndef BIKEBRAIN_WRAPPEDCFONT_H
#define BIKEBRAIN_WRAPPEDCFONT_H

#include <bikebrain/IFont.h>

namespace bikebrain
{

	class WrappedCFont : public virtual IFont
	{
	public:
		virtual stingray::Size GetSymbolSize() const;
		virtual void DrawLetter(const ILedMatrixPtr& ledMatrix, int x, int y, char c) const;
	};

}

#endif
