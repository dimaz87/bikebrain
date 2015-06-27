#include <bikebrain/platform/emu/StdinReader.h>

#include <iostream>

namespace bikebrain {
namespace emu
{

	StdinReader::StdinReader()
	{
		_worker = stingray::make_shared<stingray::Thread>("stdinReader", stingray::bind(&StdinReader::ThreadFunc, this, stingray::_1));
	}


	StdinReader::~StdinReader()
	{
		_worker.reset();
	}


	void StdinReader::ThreadFunc(const stingray::ICancellationToken& token)
	{
		while (token)
		{
			std::string s;
			std::cin >> s;
			_onInput(s);
		}
	}

}}
