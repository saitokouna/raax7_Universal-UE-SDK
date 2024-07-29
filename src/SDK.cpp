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

        if (!OffsetFinder::FindFNameConstructorNarrow())
            return SDK_FAILED_FNAMECONSTRUCTOR_NARROW;

        if (!OffsetFinder::FindFNameConstructorWide())
            return SDK_FAILED_FNAMECONSTRUCTOR_WIDE;

        if (!OffsetFinder::FindAppendString())
            return SDK_FAILED_APPENDSTRING;

        if (const auto Status = OffsetFinder::SetupMemberOffsets(); Status != SDK_SUCCESS)
            return Status;

        return OffsetFinder::FindProcessEventIdx() ? SDK_SUCCESS : SDK_FAILED_PROCESSEVENTIDX;
    }
}