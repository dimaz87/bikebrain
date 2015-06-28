#include <bikebrain/platform/edison/LedMatrix.h>


namespace bikebrain {
namespace edison
{

	LedMatrix::LedMatrix()
		: _matrix(2, 3, 4, 5, 6, 7, 8, 9)
	{ }


	stingray::Size LedMatrix::GetResolution() const
	{ return stingray::Size(32, 16); }


	void LedMatrix::EnableLed(int i, int j, bool enable)
	{ _matrix.drawPoint(i, j, enable); }

}}
