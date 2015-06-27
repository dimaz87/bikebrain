#include <bikebrain/WrappedCFont.h>

#include <bikebrain/font.c>

#include <algorithm>

namespace bikebrain
{

	stingray::Size WrappedCFont::GetSymbolSize() const
	{ return stingray::Size(8, 8); }


	void WrappedCFont::DrawLetter(const ILedMatrixPtr& ledMatrix, int x, int y, char c) const
	{
		STINGRAYKIT_CHECK(c >= 0 && c < 128, stingray::ArgumentException("c"));

		stingray::Size res = ledMatrix->GetResolution();
		if (x + 8 <= 0 || y + 8 <= 0 || x > res.Width || y > res.Height)
			return;

		for (int j = std::min(0, -y); j < std::min(8, res.Height - y); ++j)
			for (int i = std::min(0, -x); i < std::min(8, res.Width - x); ++i)
				ledMatrix->EnableLed(x + i, y + j, (font[c] & (0x1LL << ((7 - i) + (7 - j) * 8))) != 0);
	}

}
