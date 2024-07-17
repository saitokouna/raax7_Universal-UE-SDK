#include <private/Private.hpp>
#include <FMemory.hpp>
#include <libhat.hpp>

namespace Private
{
	bool SetupFMemory()
	{
		hat::fixed_signature FMemorySignature = hat::compile_signature < "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F1 41 8B D8 48 8B 0D ? ? ? ? 48 8B FA 48 85 C9">();
		const auto& Result = hat::find_pattern(FMemorySignature);

		if (!Result.has_result())
			return false;

		Realloc = (ReallocParams)Result.get();
	}
}