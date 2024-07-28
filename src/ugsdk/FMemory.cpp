#include <private/Offsets.hpp>
#include <ugsdk/FMemory.hpp>

namespace SDK::FMemory
{
    void Free(void* Original)
    {
        Realloc(Original, 0, 0);
    }
    void* Malloc(uint32_t Size, uint32_t Alignment)
    {
        return Realloc(nullptr, Size, Alignment);
    }
    void* Realloc(void* Original, uint32_t Size, uint32_t Alignment)
    {
        using ReallocParams = void* (*)(void* Original, uint32_t Size, uint32_t Alignment);
        return ReallocParams(Offsets::FMemory::Realloc)(Original, Size, Alignment);
    }
}