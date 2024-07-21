#pragma once
#include <ugsdk/UnrealEnums.hpp>
#include <cstdint>
#include <string>
#include <memory>

namespace SDK
{
	struct FUObjectItem
	{
		class UObject* Object;
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

		class UObject* GetByIndex(const int32_t Index) const;
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
		class UObject* GetByIndex(int32_t Index);

	public:
		template<typename UEType = class UObject>
		static UEType* FindObject(const std::string& FullName, EClassCastFlags RequiredType = CASTCLASS_None);

		template<typename UEType = class UObject>
		static UEType* FindObjectFast(const std::string& Name, EClassCastFlags RequiredType = CASTCLASS_None);

		template<typename UEType = class UObject>
		static UEType* FindObjectFastInOuter(const std::string& Name, const std::string& Outer);

		class UClass* FindClass(const std::string& ClassFullName)
		{
			return FindObject<class UClass>(ClassFullName, CASTCLASS_UClass);
		}

		class UClass* FindClassFast(const std::string& ClassName)
		{
			return FindObjectFast<class UClass>(ClassName, CASTCLASS_UClass);
		}
	};

	inline std::unique_ptr<TUObjectArray> GObjects = nullptr;
}

#include <ugsdk/ObjectArray.inl>