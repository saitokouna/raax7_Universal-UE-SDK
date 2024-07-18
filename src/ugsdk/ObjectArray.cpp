#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/UnrealObjects.hpp>
#include <cstdint>

namespace SDK
{
	UObject* Chunked_TUObjectArray::GetByIndex(const int32_t Index) const
	{
		if (Index < 0 || Index > NumElements)
			return nullptr;

		const int32_t ChunkIndex = Index / ElementsPerChunk;
		const int32_t InChunkIdx = Index % ElementsPerChunk;

		return GetDecrytedObjPtr()[ChunkIndex][InChunkIdx].Object;
	}
	UObject* Fixed_TUObjectArray::GetByIndex(const int32_t Index) const
	{
		if (Index < 0 || Index > NumElements)
			return nullptr;

		return GetDecrytedObjPtr()[Index].Object;
	}

	TUObjectArray::TUObjectArray(bool IsChunked, void* Objects)
	{
		m_IsChunked = IsChunked;

		if (IsChunked)
			m_ChunkedObjects = reinterpret_cast<Chunked_TUObjectArray*>(Objects);
		else
			m_FixedObjects = reinterpret_cast<Fixed_TUObjectArray*>(Objects);
	}
	int32_t TUObjectArray::Num()
	{
		if (m_IsChunked && m_ChunkedObjects)
			return m_ChunkedObjects->Num();
		else if (!m_IsChunked && m_FixedObjects)
			return m_FixedObjects->Num();

		return 0;
	}
	UObject* TUObjectArray::GetByIndex(int32_t Index)
	{
		if (m_IsChunked && m_ChunkedObjects)
			return m_ChunkedObjects->GetByIndex(Index);
		else if (!m_IsChunked && m_FixedObjects)
			return m_FixedObjects->GetByIndex(Index);

		return nullptr;
	}

	template<typename UEType>
	UEType* TUObjectArray::FindObject(const std::string& FullName, EClassCastFlags RequiredType)
	{
		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Object = GObjects->GetByIndex(i);
			if (!Object)
				continue;

			if (Object->HasTypeFlag(RequiredType) && Object->GetFullName() == FullName)
				return static_cast<UEType*>(Object);
		}

		return nullptr;
	}

	template<typename UEType>
	UEType* TUObjectArray::FindObjectFast(const std::string& Name, EClassCastFlags RequiredType)
	{
		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Object = GObjects->GetByIndex(i);
			if (!Object)
				continue;

			if (Object->HasTypeFlag(RequiredType) && Object->GetName() == Name)
				return static_cast<UEType*>(Object);
		}

		return nullptr;
	}
}