#ifndef BIKEBRAIN_ICADENCEREPORTER_H
#define BIKEBRAIN_ICADENCEREPORTER_H

#include <stingraykit/signal/signal_connector.h>

namespace bikebrain
{

	struct ICadenceReporter
	{
		virtual ~ICadenceReporter() { }

		virtual double GetCadence() const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(ICadenceReporter);

}

#endif
