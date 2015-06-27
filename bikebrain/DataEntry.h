#ifndef BIKEBRAIN_DATAENTRY_H
#define BIKEBRAIN_DATAENTRY_H

#include <stingraykit/string/ToString.h>

namespace bikebrain
{

	class DataEntry
	{
	private:
		double		_cadence;

	public:
		DataEntry(double cadence)
			: _cadence(cadence)
		{ }

		double GetCadence() const { return _cadence; }

		std::string ToString() const
		{ return stingray::StringBuilder() % "{ cadence: " % _cadence % " }"; }
	};

}

#endif
