#include <bikebrain/platform/emu/EmuLedMatrix.h>


namespace bikebrain {
namespace emu
{

	EmuLedMatrix::EmuLedMatrix(stingray::Size size)
		: _size(size), _data(size.Width * size.Height)
	{ }


	stingray::Size EmuLedMatrix::GetResolution() const
	{ return _size; }


	void EmuLedMatrix::EnableLed(int i, int j, bool enable)
	{
		STINGRAYKIT_CHECK(i >= 0 && i < _size.Width, stingray::ArgumentException("i"));
		STINGRAYKIT_CHECK(j >= 0 && j < _size.Height, stingray::ArgumentException("j"));
		_data[i + j * _size.Width] = enable;
	}

}}
