#include <bikebrain/platform/emu/EmuDisplay.h>


namespace bikebrain {
namespace emu
{

	STINGRAYKIT_DEFINE_NAMED_LOGGER(EmuDisplay);

	EmuDisplay::EmuDisplay(const std::string& name, stingray::Size size)
		:	_name(name), _size(size), _data(size.Width * size.Height),
			_deferrer(new stingray::ExecutionDeferrerWithTimer(name + "Display", 30))
	{ }


	stingray::Size EmuDisplay::GetResolution() const
	{ return _size; }


	void EmuDisplay::SetBacklightColor(RGB color) const
	{ s_logger.Info() << "SetBacklightColor(" << color << ") const"; }


	void EmuDisplay::EnableLed(int i, int j, bool enable)
	{
		STINGRAYKIT_CHECK(i >= 0 && i < _size.Width, stingray::ArgumentException("i"));
		STINGRAYKIT_CHECK(j >= 0 && j < _size.Height, stingray::ArgumentException("j"));
		_data[i + j * _size.Width] = enable;
		_deferrer->Defer(stingray::bind(&EmuDisplay::DrawContent, this));
	}


	void EmuDisplay::DrawContent()
	{
		stingray::StringBuilder sr;

		for (int j = 0; j < _size.Height; ++j)
		{
			for (int i = 0; i < _size.Width; ++i)
				sr % (_data[i + j * _size.Width] ? 'X' : '-');
			sr % "\n";
		}

		s_logger.Info() << _name << " content:\n" << sr;
	}

}}
