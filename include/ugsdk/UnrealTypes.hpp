#pragma once
#include <ostream>
#include <ugsdk/FMemory.hpp>
#include <ugsdk/Macros.hpp>

// Thanks to https://github.com/Fischsalat/UnrealContainers for proper TArray, FString and TArrayIterator support.

namespace SDK
{
    namespace Itterator
    {
        template <typename ArrayType>
        class TArrayIterator;
    }

    template <typename ArrayElementType>
    class TArray
    {
    protected:
        static constexpr uint64_t ElementAlign = alignof(ArrayElementType);
        static constexpr uint64_t ElementSize = sizeof(ArrayElementType);

    protected:
        ArrayElementType* Data;
        int32_t NumElements;
        int32_t MaxElements;

    public:
        TArray()
            : Data(nullptr)
            , NumElements(0)
            , MaxElements(0)
        {
        }

        TArray(int32_t Size)
            : Data(static_cast<ArrayElementType*>(FMemory::Malloc(Size * ElementSize, ElementAlign)))
            , NumElements(0)
            , MaxElements(Size)
        {
        }

        TArray(const TArray& Other)
            : Data(nullptr)
            , NumElements(0)
            , MaxElements(0)
        {
            this->CopyFrom(Other);
        }

        TArray(TArray&& Other) noexcept
            : Data(Other.Data)
            , NumElements(Other.NumElements)
            , MaxElements(Other.MaxElements)
        {
            Other.Data = nullptr;
            Other.NumElements = 0x0;
            Other.MaxElements = 0x0;
        }

        ~TArray()
        {
            Free();
        }

    public:
        TArray& operator=(TArray&& Other) noexcept
        {
            if (this == &Other)
                return *this;

            Free();

            Data = Other.Data;
            NumElements = Other.NumElements;
            MaxElements = Other.MaxElements;

            Other.Data = nullptr;
            Other.NumElements = 0x0;
            Other.MaxElements = 0x0;

            return *this;
        }

        TArray& operator=(const TArray& Other)
        {
            this->CopyFrom(Other);

            return *this;
        }

    private:
        inline int32_t GetSlack() const { return MaxElements - NumElements; }

        inline void VerifyIndex(int32_t Index) const
        {
            if (!IsValidIndex(Index))
                throw std::out_of_range("Index was out of range!");
        }

        inline ArrayElementType& GetUnsafe(int32_t Index) { return Data[Index]; }
        inline const ArrayElementType& GetUnsafe(int32_t Index) const { return Data[Index]; }

    public:
        inline void Reserve(int32_t Count)
        {
            if (GetSlack() < Count)
                MaxElements += Count;

            Data = static_cast<ArrayElementType*>(FMemory::Realloc(Data, MaxElements * ElementSize, ElementAlign));
        }

        inline void Add(const ArrayElementType& Element)
        {
            if (GetSlack() <= 0)
                Reserve(3);

            Data[NumElements] = Element;
            NumElements++;
        }

        inline void CopyFrom(const TArray& Other)
        {
            if (this == &Other || Other.NumElements == 0)
                return;

            NumElements = Other.NumElements;

            if (MaxElements >= Other.NumElements) {
                memcpy(Data, Other.Data, Other.NumElements);
                return;
            }

            Data = static_cast<ArrayElementType*>(FMemory::Realloc(Data, Other.NumElements * ElementSize, ElementAlign));
            MaxElements = Other.NumElements;
            memcpy(Data, Other.Data, Other.NumElements * ElementSize);
        }

        inline void Remove(int32_t Index)
        {
            if (!IsValidIndex(Index))
                return;

            NumElements--;

            for (int i = Index; i < NumElements; i++) {
                /* NumElements was decremented, acessing i + 1 is safe */
                Data[i] = Data[i + 1];
            }
        }

        inline void Clear()
        {
            NumElements = 0;

            if (!Data)
                memset(Data, 0, NumElements * ElementSize);
        }

        inline void Free() noexcept
        {
            if (Data)
                FMemory::Free(Data);

            NumElements = 0x0;
            MaxElements = 0x0;
        }

    public:
        inline int32_t Num() const { return NumElements; }
        inline int32_t Max() const { return MaxElements; }

        inline bool IsValidIndex(int32_t Index) const { return Data && Index >= 0 && Index < NumElements; }

        inline bool IsValid() const { return Data && NumElements > 0 && MaxElements >= NumElements; }

    public:
        inline ArrayElementType& operator[](int32_t Index)
        {
            VerifyIndex(Index);
            return Data[Index];
        }
        inline const ArrayElementType& operator[](int32_t Index) const
        {
            VerifyIndex(Index);
            return Data[Index];
        }

        inline bool operator==(const TArray<ArrayElementType>& Other) const { return Data == Other.Data; }
        inline bool operator!=(const TArray<ArrayElementType>& Other) const { return Data != Other.Data; }

        inline explicit operator bool() const { return IsValid(); };

    public:
        template <typename T>
        friend Itterator::TArrayIterator<T> begin(const TArray& Array);
        template <typename T>
        friend Itterator::TArrayIterator<T> end(const TArray& Array);
    };

    class FString : public TArray<wchar_t>
    {
    public:
        friend std::ostream& operator<<(std::ostream& Stream, const FString& Str) { return Stream << Str.ToString(); }

    public:
        using TArray::TArray;

        FString(const wchar_t* Str)
        {
            const uint32_t NullTerminatedLength = static_cast<uint32_t>(wcslen(Str) + 0x1);

            *this = FString(NullTerminatedLength);

            NumElements = NullTerminatedLength;
            memcpy(Data, Str, NullTerminatedLength * sizeof(wchar_t));
        }

    public:
        inline std::string ToString() const
        {
            if (*this) {
                std::wstring WData(Data);
                return std::string(WData.begin(), WData.end());
            }

            return "";
        }

        inline std::wstring ToWString() const
        {
            if (*this)
                return std::wstring(Data);

            return L"";
        }

    public:
        inline wchar_t* CStr() { return Data; }
        inline const wchar_t* CStr() const { return Data; }

    public:
        inline bool operator==(const FString& Other) const { return Other ? NumElements == Other.NumElements && wcscmp(Data, Other.Data) == 0 : false; }
        inline bool operator!=(const FString& Other) const { return Other ? NumElements != Other.NumElements || wcscmp(Data, Other.Data) != 0 : true; }
    };

    class FName
    {
    public:
        explicit FName(const std::string& Str);
        ~FName() = default;

    public:
        inline bool operator==(const FName& Other) const { return ComparisonIdx == Other.ComparisonIdx; }
        inline bool operator!=(const FName& Other) const { return ComparisonIdx != Other.ComparisonIdx; }

    public:
        uint32_t ComparisonIdx;
        uint32_t Number;

    public:
        std::string GetRawString() const;
        std::string ToString() const;
    };

    namespace Itterator
    {
        template <typename ArrayType>
        class TArrayIterator
        {
        private:
            TArray<ArrayType>& IteratedArray;
            int32_t Index;

        public:
            TArrayIterator(const TArray<ArrayType>& Array, int32_t StartIndex = 0x0)
                : IteratedArray(const_cast<TArray<ArrayType>&>(Array))
                , Index(StartIndex)
            {
            }

        public:
            inline int32_t GetIndex() { return Index; }

            inline int32_t IsValid() { return IteratedArray.IsValidIndex(GetIndex()); }

        public:
            inline TArrayIterator& operator++()
            {
                ++Index;
                return *this;
            }
            inline TArrayIterator& operator--()
            {
                --Index;
                return *this;
            }

            inline ArrayType& operator*() { return IteratedArray[GetIndex()]; }
            inline const ArrayType& operator*() const { return IteratedArray[GetIndex()]; }

            inline ArrayType* operator->() { return &IteratedArray[GetIndex()]; }
            inline const ArrayType* operator->() const { return &IteratedArray[GetIndex()]; }

            inline bool operator==(const TArrayIterator& Other) const { return &IteratedArray == &Other.IteratedArray && Index == Other.Index; }
            inline bool operator!=(const TArrayIterator& Other) const { return &IteratedArray != &Other.IteratedArray || Index != Other.Index; }
        };
    }
}