#ifndef BIKEBRAIN_PLATFORM_EMU_STDINREADER_H
#define BIKEBRAIN_PLATFORM_EMU_STDINREADER_H

#include <stingraykit/signal/signals.h>
#include <stingraykit/thread/CancellationToken.h>
#include <stingraykit/thread/Thread.h>
#include <stingraykit/toolkit.h>

namespace bikebrain {
namespace emu
{

	class StdinReader
	{
		STINGRAYKIT_NONCOPYABLE(StdinReader);

	private:
		stingray::signal<void(const std::string&)>		_onInput;
		stingray::ThreadPtr								_worker;

	public:
		StdinReader();
		~StdinReader();

		stingray::signal_connector<void(const std::string&)> OnInput() const
		{ return _onInput.connector(); }

	private:
		void ThreadFunc(const stingray::ICancellationToken& token);
	};
	STINGRAYKIT_DECLARE_PTR(StdinReader);

}}

#endif
