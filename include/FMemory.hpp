#pragma once
#include <cstdint>

namespace SDK::FMemory
{
    void Free(void* Original);
    void* Malloc(uint32_t Size, uint32_t Alignment);
    void* Realloc(void* Original, uint32_t Size, uint32_t Alignment);
}