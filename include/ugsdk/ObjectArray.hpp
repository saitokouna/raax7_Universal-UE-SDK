#pragma once
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
	struct FUObjectItem
	{
		UObject* Object;
		uint8_t Pad_0[0x10];
	};

	class Chunked_TUObjectArray
	{
	private:
		enum
		{
			ElementsPerChunk = 0x10000,
		};

	private:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8_t*
			{
				return reinterpret_cast<uint8_t*>(ObjPtr);
			};

	private:
		FUObjectItem** Objects;
		uint8_t Pad_0[0x08];
		int32_t MaxElements;
		int32_t NumElements;
		int32_t MaxChunks;
		int32_t NumChunks;

	public:
		inline int32_t Num() const { return NumElements; }
		inline FUObjectItem** GetDecrytedObjPtr() const { return reinterpret_cast<FUObjectItem**>(DecryptPtr(Objects)); }

		class UObject* GetByIndex(const int32_t Index) const;
	};
	class Fixed_TUObjectArray
	{
	private:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8_t*
			{
				return reinterpret_cast<uint8_t*>(ObjPtr);
			};

	private:
		FUObjectItem* Objects;
		int32_t MaxElements;
		int32_t NumElements;

	public:
		inline int Num() const { return NumElements; }
		inline FUObjectItem* GetDecrytedObjPtr() const { return reinterpret_cast<FUObjectItem*>(DecryptPtr(Objects)); }

		UObject* GetByIndex(const int32_t Index) const;
	};

	class TUObjectArray
	{
	public:
		TUObjectArray(bool IsChunked, void* Objects);

	private:
		bool m_IsChunked;
		Chunked_TUObjectArray* m_ChunkedObjects;
		Fixed_TUObjectArray* m_FixedObjects;

	public:
		int32_t Num();
		UObject* GetByIndex(int32_t Index);
	};
}