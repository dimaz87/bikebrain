#ifndef BIKEBRAIN_PLATFORM_EDISON_GPIO_H
#define BIKEBRAIN_PLATFORM_EDISON_GPIO_H

#include <stingraykit/shared_ptr.h>
#include <stingraykit/toolkit.h>
#include <stingraykit/signal/signal_connector.h>

namespace bikebrain {
namespace edison
{

	class Gpio
	{
	public:
		struct Direction
		{
			STINGRAYKIT_ENUM_VALUES(In, Out);
			STINGRAYKIT_DECLARE_ENUM_CLASS(Direction);
		};

		struct Edge
		{
			STINGRAYKIT_ENUM_VALUES(Rising, Falling, Both);
			STINGRAYKIT_DECLARE_ENUM_CLASS(Edge);
		};

	private:
		struct Impl;
		STINGRAYKIT_DECLARE_PTR(Impl);

	private:
		ImplPtr		_impl;

	public:
		Gpio(int portNumber, Direction dir);
		Gpio(int portNUmber, Edge edge);

		bool Read() const;
		void Write(bool value);

		stingray::signal_connector<void (bool)> OnEvent() const;
	};

}}

#endif
