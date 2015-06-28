#include <bikebrain/platform/edison/Gpio.h>

#include <bikebrain/platform/edison/MraaCall.h>

#include <stingraykit/exception.h>
#include <stingraykit/Mapper.h>
#include <stingraykit/log/Logger.h>
#include <stingraykit/signal/signals.h>

#include <mraa.hpp>

using namespace stingray;

namespace bikebrain {
namespace edison
{

	struct OutputMode
	{
		STINGRAYKIT_ENUM_VALUES(Strong, PullUp, PullDown, HighZ);
		STINGRAYKIT_DECLARE_ENUM_CLASS(OutputMode);
	};


	struct OutputModeMapper : public BaseValueMapper<OutputModeMapper, OutputMode::Enum, mraa::Mode>
	{
		typedef TypeList<
				Src::Value<OutputMode::Strong>  , Dst::Value<mraa::MODE_STRONG>,
				Src::Value<OutputMode::PullUp>  , Dst::Value<mraa::MODE_PULLUP>,
				Src::Value<OutputMode::PullDown>, Dst::Value<mraa::MODE_PULLDOWN>,
				Src::Value<OutputMode::HighZ>   , Dst::Value<mraa::MODE_HIZ> >::type	MappingsList;

		typedef TypeList<Src::Fail, Dst::Fail>	DefaultMapping;
	};


	struct DirectionMapper : public BaseValueMapper<DirectionMapper, Gpio::Direction::Enum, mraa::Dir>
	{
		typedef TypeList<
				Src::Value<Gpio::Direction::In>, Dst::Value<mraa::DIR_IN>,
				Src::Value<Gpio::Direction::Out>, Dst::Value<mraa::DIR_OUT> >::type		MappingsList;

		typedef TypeList<Src::Fail, Dst::Fail>	DefaultMapping;
	};


	struct EdgeMapper : public BaseValueMapper<EdgeMapper, Gpio::Edge::Enum, mraa::Edge>
	{
		typedef TypeList<
				Src::Value<Gpio::Edge::Rising>, Dst::Value<mraa::EDGE_RISING>,
				Src::Value<Gpio::Edge::Falling>, Dst::Value<mraa::EDGE_FALLING>,
				Src::Value<Gpio::Edge::Both>, Dst::Value<mraa::EDGE_BOTH> >::type		MappingsList;

		typedef TypeList<Src::Fail, Dst::Value<mraa::EDGE_NONE> >	DefaultMapping;
	};


	class Gpio::Impl
	{
	private:
		int						_pin;
		mutable mraa::Gpio		_gpio;
		signal<void (bool)>		_onEvent;

	public:
		Impl(int portNumber) : _pin(portNumber), _gpio(_pin, true, false)
		{

		}

		~Impl()
		{
			_gpio.isr(mraa::EDGE_NONE, NULL, NULL);
		}

		int GetPinNumber() const { return _pin; }

		void SetDirection(Gpio::Direction direction)
		{
			MRAA_CALL(_gpio.dir(DirectionMapper::Map(direction)));
			if (direction == Gpio::Direction::Out)
				MRAA_CALL(_gpio.mode(OutputModeMapper::Map(OutputMode::Strong)));
		}

		void SetEdge(Gpio::Edge edge)
		{
			MRAA_CALL(_gpio.isr(EdgeMapper::Map(edge), &Impl::IsrHandler, this));
		}

		bool Read() const
		{
			return _gpio.read();
		}

		void Write(bool value)
		{
			MRAA_CALL(_gpio.write(value));
		}

		signal_connector<void (bool)> OnEvent() const
		{
			return _onEvent.connector();
		}

	private:
		static void IsrHandler(void* arg)
		{
			try
			{
				Logger::Info() << "ISR HANDLER";

				Impl* impl = static_cast<Impl*>(arg);
				impl->_onEvent(impl->Read());
			}
			catch (const std::exception& ex)
			{
				perror(ex.what());
			}
		}
	};


	Gpio::Gpio(int portNumber, Direction dir) : _port(portNumber), _impl(new Impl(portNumber))
	{
		Logger::Info() << "Try to open GPIO on port " << portNumber << " with direction " << dir;
		_impl->SetDirection(dir);
	}


	Gpio::Gpio(int portNumber, Edge edge) : _port(portNumber), _impl(new Impl(portNumber))
	{
		Logger::Info() << "Try to open GPIO on port " << portNumber << " with edge control " << edge;
		_impl->SetDirection(Gpio::Direction::In);
		_impl->SetEdge(edge);
	}


	Gpio::~Gpio()
	{
		Logger::Info() << "Destroying GPIO, port: " << _port << ", pin: " << _impl->GetPinNumber();
	}


	bool Gpio::Read() const
	{
		return _impl->Read();
	}


	void Gpio::Write(bool value)
	{
		_impl->Write(value);
	}


	signal_connector<void (bool)> Gpio::OnEvent() const
	{
		return _impl->OnEvent();
	}

}}
