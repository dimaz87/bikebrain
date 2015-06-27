#include <bikebrain/platform/emu/EmuButton.h>


namespace bikebrain {
namespace emu
{

	EmuButton::EmuButton(const StdinReaderPtr& stdinReader, const std::string& name)
		: _stdinReader(stdinReader), _name(name)
	{ _tokens += _stdinReader->OnInput().connect(bind(&EmuButton::StdinInputHandler, this, stingray::_1)); }


	EmuButton::~EmuButton()
	{ _tokens.Release(); }


	void EmuButton::StdinInputHandler(const std::string& input)
	{
		if (input == _name)
			_onPressed();
	}

}}
