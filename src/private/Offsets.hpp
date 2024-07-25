#pragma once
#include <ugsdk/Macros.hpp>
#include <cstdint>

namespace SDK::Offsets
{
	using Offset_t = uint16_t;

	namespace UObject
	{
		constexpr Offset_t VFT = 0; // This will always be 0.
		inline Offset_t Flags = 8;
		inline Offset_t Index = 0xC;
		inline Offset_t Class = 0x10;
		inline Offset_t Name = 0x18;
		inline Offset_t Outer = 0x20;
	}
	namespace UField
	{
		inline Offset_t Next = OFFSET_NOT_FOUND;
	}
	namespace UStruct
	{
		inline Offset_t Super = OFFSET_NOT_FOUND;
		inline Offset_t Children = OFFSET_NOT_FOUND;
		inline Offset_t ChildProperties = OFFSET_NOT_FOUND;
	}
	namespace UClass
	{
		inline Offset_t CastFlags = OFFSET_NOT_FOUND;
		inline Offset_t DefaultObject = OFFSET_NOT_FOUND;
	}
	namespace UProperty
	{
		inline Offset_t Offset = OFFSET_NOT_FOUND;
		inline Offset_t ElementSize = OFFSET_NOT_FOUND;
		inline Offset_t PropertyFlags = OFFSET_NOT_FOUND;
	}
	namespace UBoolProperty
	{
		struct UBoolPropertyBase
		{
			uint8_t FieldSize;
			uint8_t ByteOffset;
			uint8_t ByteMask;
			uint8_t FieldMask;
		};

		inline Offset_t Base = OFFSET_NOT_FOUND;
	}
	namespace UFunction
	{
		inline Offset_t NumParms = OFFSET_NOT_FOUND;
		inline Offset_t ParmsSize = OFFSET_NOT_FOUND;
		inline Offset_t ReturnValueOffset = OFFSET_NOT_FOUND;
	}

	namespace FMemory
	{
		inline uintptr_t Realloc = 0;
	}
	namespace FName
	{
		inline uintptr_t Constructor = 0;
		inline uintptr_t AppendString = 0;
	}
}