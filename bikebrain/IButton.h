#ifndef BIKEBRAIN_IBUTTON_H
#define BIKEBRAIN_IBUTTON_H

#include <stingraykit/signal/signal_connector.h>

namespace bikebrain
{

	struct IButton
	{
		virtual ~IButton() { }

		virtual stingray::signal_connector<void()> OnPressed() const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IButton);

}

#endif
