#ifndef BIKEBRAIN_PLATFORM_EDISON_LEDMATRIX_H
#define BIKEBRAIN_PLATFORM_EDISON_LEDMATRIX_H

#include <bikebrain/ILedMatrix.h>

#include <Ultrathin_LED_Matrix/LEDMatrix.h>


namespace bikebrain {
namespace edison
{

	class LedMatrix : public virtual ILedMatrix
	{
	private:
		LEDMatrix	_matrix;

	public:
		LedMatrix();

		virtual stingray::Size GetResolution() const = 0;
		virtual void EnableLed(int i, int j, bool enable) = 0;
	};

}}

#endif
