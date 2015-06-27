#ifndef BIKEBRAIN_PLATFORM_EDISON_BUTTON_H
#define BIKEBRAIN_PLATFORM_EDISON_BUTTON_H

#include <bikebrain/IButton.h>

#include <stingraykit/shared_ptr.h>

namespace bikebrain {
namespace edison
{

	class Button : public virtual IButton
	{
		struct ButtonImpl;
		STINGRAYKIT_DECLARE_PTR(ButtonImpl);

	private:
		ButtonImplPtr	_impl;

	public:
		Button();
		~Button();

		virtual stingray::signal_connector<void()> OnPressed() const;

	private:
	};

}}

#endif
