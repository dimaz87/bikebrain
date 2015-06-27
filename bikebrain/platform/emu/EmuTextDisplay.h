#ifndef BIKEBRAIN_PLATFORM_EMU_EMUTEXTDISPLAY_H
#define BIKEBRAIN_PLATFORM_EMU_EMUTEXTDISPLAY_H

#include <bikebrain/ITextDisplay.h>

#include <stingraykit/log/Logger.h>

namespace bikebrain {
namespace emu
{

	class EmuTextDisplay : public virtual ITextDisplay
	{
	private:
		static stingray::NamedLogger				s_logger;

	public:
		virtual void SetBacklightColor(RGB color) const;
		virtual void SetText(const std::string& str) const;
	};

}}

#endif
