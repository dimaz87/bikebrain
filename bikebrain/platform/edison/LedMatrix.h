#ifndef BIKEBRAIN_PLATFORM_EDISON_LEDMATRIX_H
#define BIKEBRAIN_PLATFORM_EDISON_LEDMATRIX_H

#include <bikebrain/ILedMatrix.h>

#include <Ultrathin_LED_Matrix/LEDMatrix.h>

#include <stingraykit/thread/Thread.h>


namespace bikebrain {
namespace edison
{

	class LedMatrix : public virtual ILedMatrix
	{
	private:
		LEDMatrix	_matrix;
		std::vector<stingray::u8>	_displayBuf;
		stingray::Mutex		_sync;
		stingray::ThreadPtr	_thread;

	public:
		LedMatrix();

		virtual stingray::Size GetResolution() const;
		virtual void EnableLed(int i, int j, bool enable);

	private:
		void ThreadFunc(const stingray::ICancellationToken& token);
	};

}}

#endif
