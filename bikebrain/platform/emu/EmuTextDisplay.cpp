#include <bikebrain/platform/emu/EmuTextDisplay.h>


namespace bikebrain {
namespace emu
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(EmuTextDisplay);


	void EmuTextDisplay::SetBacklightColor(RGB color) const
	{ s_logger.Info() << "SetBacklightColor(" << color << ") const"; }


	void EmuTextDisplay::SetText(const std::string& str) const
	{ s_logger.Info() << "SetText(" << str << ") const"; }

}}
