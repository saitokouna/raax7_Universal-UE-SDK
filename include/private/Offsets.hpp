#pragma once
#include <cstdint>

#define OFFSET_NOT_FOUND -1

/** @brief This namespace is managed by the library and not meant for direct use. */
namespace SDK::Offsets
{
	using Offset = uint16_t;

	namespace UObject
	{
		inline Offset VFT = 0;
		inline Offset Flags = 8;
		inline Offset Index = 0xC;
		inline Offset Class = 0x10;
		inline Offset Name = 0x18;
		inline Offset Outer = 0x20;
	}
	namespace UField
	{
		inline Offset Next = OFFSET_NOT_FOUND;
	}
	namespace UStruct
	{
		inline Offset Super = OFFSET_NOT_FOUND;
		inline Offset Children = OFFSET_NOT_FOUND;
		inline Offset ChildProperties = OFFSET_NOT_FOUND;
	}
	namespace UClass
	{
		inline Offset CastFlags = OFFSET_NOT_FOUND;
		inline Offset DefaultObject = OFFSET_NOT_FOUND;
	}

	namespace FMemory
	{
		inline uintptr_t Realloc = 0;
	}
}