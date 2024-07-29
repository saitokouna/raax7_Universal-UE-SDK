#pragma once

namespace OffsetFinder
{
    bool FindFMemoryRealloc();
    bool FindGObjects();
    bool FindFNameConstructorNarrow();
    bool FindFNameConstructorWide();
    bool FindAppendString();
    bool FindProcessEventIdx();

    SDK::SDKStatus SetupMemberOffsets();
}