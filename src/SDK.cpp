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

        if (!OffsetFinder::FindFNameConstructor())
            return SDK_FAILED_FNAMECONSTRUCTOR;

        if (!OffsetFinder::FindAppendString())
            return SDK_FAILED_APPENDSTRING;

        SDKStatus Status = OffsetFinder::SetupMemberOffsets();
        if (Status != SDK_SUCCESS)
            return Status;

        return OffsetFinder::FindProcessEventIdx() ? SDK_SUCCESS : SDK_FAILED_PROCESSEVENTIDX;
    }
}