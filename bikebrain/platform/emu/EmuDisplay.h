#ifndef BIKEBRAIN_PLATFORM_EMU_EMUDISPLAY_H
#define BIKEBRAIN_PLATFORM_EMU_EMUDISPLAY_H

#include <bikebrain/IDisplay.h>

#include <stingraykit/timer/Timer.h>

#include <string>
#include <vector>

namespace bikebrain {
namespace emu
{

	class EmuDisplay : public virtual IDisplay
	{
	private:
		static stingray::NamedLogger				s_logger;

		std::string									_name;
		stingray::Size								_size;
		std::vector<bool>							_data;

		stingray::ExecutionDeferrerWithTimerPtr		_deferrer;

	public:
		EmuDisplay(const std::string& name, stingray::Size size);

		virtual void SetBacklightColor(RGB color) const;
		virtual stingray::Size GetResolution() const;
		virtual void EnableLed(int i, int j, bool enable);

	private:
		void DrawContent();
	};

}}

#endif
