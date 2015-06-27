#ifndef BIKEBRAIN_PLATFORM_EMU_STATSENGINE_H
#define BIKEBRAIN_PLATFORM_EMU_STATSENGINE_H

#include <bikebrain/IStatsEngine.h>

#include <stingraykit/log/Logger.h>

namespace bikebrain {
namespace emu
{

	class EmuStatsEngine : public virtual IStatsEngine
	{
	private:
		static stingray::NamedLogger		s_logger;

	public:
		EmuStatsEngine();
		~EmuStatsEngine();

		virtual ITripPtr StartTrip();
	};

}}

#endif
