#pragma once

namespace OffsetFinder
{
	bool FindFMemoryRealloc();
	bool FindGObjects();
	bool FindAppendString();

	SDK::SDKStatus SetupMemberOffsets();
}