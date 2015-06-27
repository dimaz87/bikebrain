#ifndef BIKEBRAIN_IFONT_H
#define BIKEBRAIN_IFONT_H

#include <bikebrain/ILedMatrix.h>

#include <stingraykit/Rect.h>
#include <stingraykit/exception.h>

#include <string>

namespace bikebrain
{

	struct IFont
	{
		virtual ~IFont() { }

		virtual stingray::Size GetSymbolSize() const = 0;
		virtual void DrawLetter(const ILedMatrixPtr& ledMatrix, int x, int y, char c) const = 0;

		virtual void DrawString(const ILedMatrixPtr& ledMatrix, int x, int y, const std::string& str) const
		{
			for (int i = 0; i < str.size(); ++i)
				DrawLetter(ledMatrix, x + i * GetSymbolSize().Width, y, str[i]);
		}
	};
	STINGRAYKIT_DECLARE_PTR(IFont);

}

#endif
