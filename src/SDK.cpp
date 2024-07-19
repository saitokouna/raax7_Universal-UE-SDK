#include <SDK.hpp>
#include <private/OffsetFinder.hpp>

namespace SDK
{
	SDKStatus Init()
	{
		if (!OffsetFinder::FindFMemoryRealloc())
			return SDK_FAILED_FMEMORY_REALLOC;

		if (!OffsetFinder::FindGObjects())
			return SDK_FAILED_GOBJECTS;

		if (!OffsetFinder::FindAppendString())
			return SDK_FAILED_APPENDSTRING;

		return OffsetFinder::SetupMemberOffsets();
	}
}