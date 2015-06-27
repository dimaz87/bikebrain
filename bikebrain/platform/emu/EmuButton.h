#ifndef BIKEBRAIN_PLATFORM_EMU_EMUBUTTON_H
#define BIKEBRAIN_PLATFORM_EMU_EMUBUTTON_H

#include <bikebrain/IButton.h>
#include <bikebrain/platform/emu/StdinReader.h>

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

		StdinReaderPtr						_stdinReader;
		std::string							_name;
		stingray::signal<void()>			_onPressed;

		stingray::TokenPool					_tokens;

	public:
		EmuButton(const StdinReaderPtr& stdinReader, const std::string& name);
		~EmuButton();

		virtual stingray::signal_connector<void()> OnPressed() const
		{ return _onPressed.connector(); }

	private:
		void StdinInputHandler(const std::string& input);
	};
}}

#endif
