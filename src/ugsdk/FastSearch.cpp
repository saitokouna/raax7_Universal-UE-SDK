#include <ugsdk/FastSearch.hpp>
#include <private/Settings.hpp>
#include <ugsdk/ObjectArray.hpp>

namespace SDK
{
	bool FastSearch(const std::vector<FSEntry>& SearchList)
	{
		// We require all of these functionalities.
		if (!Settings::SetupFMemory ||
			!Settings::SetupGObjects ||
			!Settings::SetupAppendString ||
			!Settings::SetupFNameConstructor)
		{
			return false;
		}

		// If we have not setup member offsets, we can only scan for UObjects.
		if (!Settings::SetupMemberOffsets)
		{
			for (const auto& Search : SearchList)
			{
				if (Search.Type != FS_UOBJECT)
					return false;
			}
		}

		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Obj = GObjects->GetByIndex(i);
			if (!Obj)
				continue;


		}
	}
}