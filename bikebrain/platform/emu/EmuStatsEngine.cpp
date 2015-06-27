#include <bikebrain/platform/emu/EmuStatsEngine.h>


namespace bikebrain {
namespace emu
{

	class EmuTrip : public virtual ITrip
	{
	private:
		static stingray::NamedLogger		s_logger;

	public:
		EmuTrip()
		{ s_logger.Info() << "Created"; }

		~EmuTrip()
		{ s_logger.Info() << "Destroying"; }

		virtual void ReportDataEntry(const DataEntry& dataEntry)
		{ s_logger.Info() << "ReportDataEntry(" << dataEntry << ")"; }
	};

	STINGRAYKIT_DEFINE_NAMED_LOGGER(EmuTrip);


	STINGRAYKIT_DEFINE_NAMED_LOGGER(EmuStatsEngine);

	EmuStatsEngine::EmuStatsEngine()
	{ s_logger.Info() << "Created"; }


	EmuStatsEngine::~EmuStatsEngine()
	{ s_logger.Info() << "Destroying"; }


	ITripPtr EmuStatsEngine::StartTrip()
	{ return stingray::make_shared<EmuTrip>(); }

}}
