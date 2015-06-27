#ifndef BIKEBRAIN_PLATFORM_EMU_EMUBUTTON_H
#define BIKEBRAIN_PLATFORM_EMU_EMUBUTTON_H

#include <bikebrain/IButton.h>

#include <stingraykit/log/Logger.h>
#include <stingraykit/signal/signals.h>

#include <string>

namespace bikebrain {
namespace emu
{

	class EmuButton : public virtual IButton
	{
	private:
		static stingray::NamedLogger		s_logger;

		std::string							_name;
		stingray::signal<void()>			_onPressed;

	public:
		EmuButton(const std::string& name);

		virtual stingray::signal_connector<void()> OnPressed() const
		{ return _onPressed.connector(); }
	};
}}

#endif
