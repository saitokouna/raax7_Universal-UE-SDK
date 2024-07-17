#include <SDK.hpp>
#include <private/OffsetFinder.hpp>

namespace SDK
{
	SDKStatus Init()
	{
		if (!OffsetFinder::FindFMemoryRealloc())
			return SDK_FAILED_FMEMORY_REALLOC;

		return SDK_SUCCESS;
	}
}