#ifndef BIKEBRAIN_ITEXTDISPLAY_H
#define BIKEBRAIN_ITEXTDISPLAY_H

#include <bikebrain/RGB.h>

#include <stingraykit/shared_ptr.h>

namespace bikebrain
{

	struct ITextDisplay
	{
		virtual ~ITextDisplay() { }

		virtual void SetBacklightColor(RGB color) const = 0;
		virtual void SetText(const std::string& str) const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(ITextDisplay);

}

#endif
