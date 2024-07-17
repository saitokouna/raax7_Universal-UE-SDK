#pragma once
#include <cstdint>

namespace Private
{
	using ReallocParams = void* (*)(void* Original, uint32_t Size, uint32_t Alignment);
	inline ReallocParams Realloc = nullptr;

	bool SetupFMemory();
}