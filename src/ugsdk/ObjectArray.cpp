#include <cstdint>
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
    FUObjectItem* Chunked_TUObjectArray::IndexToObject(int32_t Index) const
    {
        if (Index < 0 || Index > NumElements)
            return nullptr;

        const int32_t ChunkIndex = Index / ElementsPerChunk;
        const int32_t InChunkIdx = Index % ElementsPerChunk;

        return &GetDecrytedObjPtr()[ChunkIndex][InChunkIdx];
    }
    FUObjectItem* Fixed_TUObjectArray::IndexToObject(int32_t Index) const
    {
        if (Index < 0 || Index > NumElements)
            return nullptr;

        return &GetDecrytedObjPtr()[Index];
    }

    TUObjectArray::TUObjectArray(bool IsChunked, void* Objects)
        : m_IsChunked(IsChunked)
        , m_ChunkedObjects(nullptr)
    {
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
    FUObjectItem* TUObjectArray::IndexToObject(int32_t Index)
    {
        if (Index < 0 || Index >= Num())
            return nullptr;

        if (m_IsChunked)
            return m_ChunkedObjects->IndexToObject(Index);
        else
            return m_FixedObjects->IndexToObject(Index);

        return nullptr;
    }

    class UObject* TUObjectArray::GetByIndex(int32_t Index)
    {
        FUObjectItem* Object = IndexToObject(Index);
        if (!Object)
            return nullptr;

        return Object->Object;
    }
}