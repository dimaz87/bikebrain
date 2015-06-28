#ifndef BIKEBRAIN_PLATFORM_EDISON_MRAACALL_H
#define BIKEBRAIN_PLATFORM_EDISON_MRAACALL_H

#include <stingraykit/exception.h>
#include <stingraykit/string/ToString.h>

#include <string>

#include <mraa.hpp>

#define MRAA_CALL(...) \
	do \
	{ \
		mraa_result_t result = (__VA_ARGS__); \
		STINGRAYKIT_CHECK(result == MRAA_SUCCESS, #__VA_ARGS__ " failed: " + MraaResultToString(result)); \
	} while (0)

namespace bikebrain {
namespace edison
{

	std::string MraaResultToString(mraa_result_t result)
	{
		switch (result)
		{
#define MRAA_ERR_TO_STR(errCode) case errCode: return #errCode;
		MRAA_ERR_TO_STR(MRAA_SUCCESS);
		MRAA_ERR_TO_STR(MRAA_ERROR_FEATURE_NOT_IMPLEMENTED);
		MRAA_ERR_TO_STR(MRAA_ERROR_FEATURE_NOT_SUPPORTED);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_VERBOSITY_LEVEL);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_PARAMETER);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_HANDLE);
		MRAA_ERR_TO_STR(MRAA_ERROR_NO_RESOURCES);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_RESOURCE);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_QUEUE_TYPE);
		MRAA_ERR_TO_STR(MRAA_ERROR_NO_DATA_AVAILABLE);
		MRAA_ERR_TO_STR(MRAA_ERROR_INVALID_PLATFORM);
		MRAA_ERR_TO_STR(MRAA_ERROR_PLATFORM_NOT_INITIALISED);
		MRAA_ERR_TO_STR(MRAA_ERROR_PLATFORM_ALREADY_INITIALISED);
		MRAA_ERR_TO_STR(MRAA_ERROR_UNSPECIFIED);
#undef MRAA_ERR_TO_STR
		default:
			return stingray::ToString(result);
		}
	}

}}

#endif
