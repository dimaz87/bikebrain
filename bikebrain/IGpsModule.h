#ifndef BIKEBRAIN_IGPSMODULE_H
#define BIKEBRAIN_IGPSMODULE_H

#include <bikebrain/GpsData.h>

namespace bikebrain
{

	struct IGpsModule
	{
		virtual ~IGpsModule() { }

		virtual GpsData GetData() const = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IGpsModule);

}

#endif
