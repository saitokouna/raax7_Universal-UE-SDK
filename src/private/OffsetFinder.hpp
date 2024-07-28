#pragma once

namespace OffsetFinder
{
    bool FindFMemoryRealloc();
    bool FindGObjects();
    bool FindFNameConstructor();
    bool FindAppendString();
    bool FindProcessEventIdx();

    SDK::SDKStatus SetupMemberOffsets();
}