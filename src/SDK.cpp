#include <UGSDK.hpp>
#include <private/OffsetFinder.hpp>

namespace SDK
{
    Status Init()
    {
        if (!OffsetFinder::FindFMemoryRealloc())
            return Status::FMemoryRealloc;

        if (!OffsetFinder::FindGObjects())
            return Status::GObjects;

        if (!OffsetFinder::FindFNameConstructorNarrow())
            return Status::NarrowFNameConstructor;

        if (!OffsetFinder::FindFNameConstructorWide())
            return Status::WideFNameConstructor;

        if (!OffsetFinder::FindAppendString())
            return Status::AppendString;

        if (const auto Status = OffsetFinder::SetupMemberOffsets(); Status != Status::Success)
            return Status;

        if (!OffsetFinder::FindConsoleCommandIdx())
            return Status::ConsoleCommand;

        if (!OffsetFinder::FindProcessEventIdx())
            return Status::ProcessEvent;

        return Status::Success;
    }
}