#include <bikebrain/platform/edison/LedMatrix.h>

#include <stingraykit/function/bind.h>
#include <stingraykit/log/Logger.h>

using namespace stingray;

namespace bikebrain {
namespace edison
{

	LedMatrix::LedMatrix()
		: _matrix(5, 4, 3, 2, 8, 9, 6, 7), _displayBuf(32*16)
	{
		_matrix.begin(_displayBuf.data(), 32, 16);
		_matrix.clear();
		_thread.reset(new stingray::Thread("ledMatrix", stingray::bind(&LedMatrix::ThreadFunc, this, stingray::_1)));
	}


	stingray::Size LedMatrix::GetResolution() const
	{ return stingray::Size(32, 16); }


	void LedMatrix::EnableLed(int i, int j, bool enable)
	{
		stingray::MutexLock l(_sync);
		_changes.push_back(Change(i, j, enable));
	}


	void LedMatrix::ThreadFunc(const stingray::ICancellationToken& token)
	{
		while (token)
		{
			{
				stingray::MutexLock l(_sync);
				while (!_changes.empty())
				{
					Change c = _changes.back();
					_matrix.drawPoint(c.Get<0>(), c.Get<1>(), c.Get<2>());
					_changes.pop_back();
				}
			}
			_matrix.scan();
		}
	}

}}
