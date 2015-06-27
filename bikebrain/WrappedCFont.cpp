#include <bikebrain/WrappedCFont.h>

#include <bikebrain/font.c>


namespace bikebrain
{

	stingray::Size WrappedCFont::GetSymbolSize() const
	{ return stingray::Size(8, 8); }


	void WrappedCFont::DrawLetter(const ILedMatrixPtr& ledMatrix, int x, int y, char c) const
	{
		STINGRAYKIT_CHECK(c >=0 && c < 128, stingray::ArgumentException("c"));
		STINGRAYKIT_CHECK(x >=0 && x + 8 < ledMatrix->GetResolution().Width, stingray::ArgumentException("x"));
		STINGRAYKIT_CHECK(y >=0 && y + 8 < ledMatrix->GetResolution().Height, stingray::ArgumentException("y"));

		for (int j = 0; j < 8; ++j)
			for (int i = 0; i < 8; ++i)
				ledMatrix->EnableLed(x + i, y + j, (font[c] & (0x1LL << ((7 - i) + (7 - j) * 8))) != 0);
	}

}
