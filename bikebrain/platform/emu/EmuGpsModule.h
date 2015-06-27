#ifndef BIKEBRAIN_PLATFORM_EMU_EMUGPSMODULE_H
#define BIKEBRAIN_PLATFORM_EMU_EMUGPSMODULE_H

#include <bikebrain/IGpsModule.h>

namespace bikebrain {
namespace emu
{

	class EmuGpsModule : public virtual IGpsModule
	{
	public:
		virtual GpsData GetData() const;
	};

}}

#endif
