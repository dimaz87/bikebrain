#ifndef BIKEBRAIN_ICADENCEREPORTER_H
#define BIKEBRAIN_ICADENCEREPORTER_H

#include <stingraykit/signal/signal_connector.h>

namespace bikebrain
{

	struct ICadenceReporter
	{
		virtual ~ICadenceReporter() { }

		virtual stingray::signal_connector<void(double)> OnCadence() const;
	};
	STINGRAYKIT_DECLARE_PTR(ICadenceReporter);

}

#endif
