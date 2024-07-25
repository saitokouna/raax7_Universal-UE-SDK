#pragma once

namespace OffsetFinder
{
	bool FindFMemoryRealloc();
	bool FindGObjects();
	bool FindFNameConstructor();
	bool FindAppendString();

	SDK::SDKStatus SetupMemberOffsets();
}