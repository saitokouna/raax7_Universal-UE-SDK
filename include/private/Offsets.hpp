#pragma once
#include <cstdint>

/** @brief This namespace is managed by the library and not meant for direct use. */
namespace Offsets
{
	using Offset = uint16_t;
	constexpr uint16_t OffsetNotFound = -1;

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
		inline Offset Next = OffsetNotFound;
	}

	namespace UStruct
	{
		inline Offset Super = OffsetNotFound;
		inline Offset Children = OffsetNotFound;
		inline Offset ChildProperties = OffsetNotFound;
	}

	namespace UClass
	{
		inline Offset CastFlags = OffsetNotFound;
		inline Offset DefaultObject = OffsetNotFound;
	}
}