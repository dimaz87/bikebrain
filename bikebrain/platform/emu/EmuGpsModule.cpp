#include <bikebrain/platform/emu/EmuGpsModule.h>


namespace bikebrain {
namespace emu
{

	GpsData EmuGpsModule::GetData() const
	{
		return GpsData(0, 0, 0, stingray::Time::Now());
	}

}}
