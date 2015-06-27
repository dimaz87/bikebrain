#ifndef BIKEBRAIN_PLATFORM_EMU_EMULEDMATRIX_H
#define BIKEBRAIN_PLATFORM_EMU_EMULEDMATRIX_H

#include <bikebrain/ILedMatrix.h>

#include <vector>

namespace bikebrain {
namespace emu
{

	class EmuLedMatrix : public virtual ILedMatrix
	{
	private:
		stingray::Size			_size;
		std::vector<bool>		_data;

	public:
		EmuLedMatrix(stingray::Size size);

		virtual stingray::Size GetResolution() const;
		virtual void EnableLed(int i, int j, bool enable);
	};

}}

#endif
