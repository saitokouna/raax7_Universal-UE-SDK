#pragma once
#include <ugsdk/Macros.hpp>
#include <libhat.hpp>
#include <Windows.h>
#include <vector>
#include <functional>

// Credit to https://github.com/Encryqed/Dumper-7 for the FindOffset function.

namespace SDK::Memory
{
    uintptr_t CalculateRVA(uintptr_t Addr, uint32_t Offset);
    bool Is32BitRelativeAddress(uint8_t ModRM);

    std::byte* ItterateAll(hat::signature_view Signature, const std::string& Section, const std::function<bool(std::byte*)>& It);
    std::byte* FindPatternInRange(const std::byte* Start, const std::byte* End, hat::signature_view Signature);

    template<typename T>
    inline std::byte* FindString(const T* String) {
        const auto StringSig = hat::string_to_signature(std::basic_string_view<T>(String));
        return (std::byte*)hat::find_pattern(StringSig, ".rdata").get();
    }

    template<typename T>
    inline std::byte* FindStringRef(const T* String) {
        std::byte* StringAddr = FindString(String);
        if (!StringAddr)
            return nullptr;

        // The encoding of our target LEA is:
        //
        // 48 - REX prefix (indicating 64-bit operand size)
        // 8D - LEA
        // ?? - ModR/M byte
        // ?? ?? ?? ?? - Address
        //
        // We will use Is32BitRelativeAddress to check if the ModR/M byte is correct.

        const auto Validate = [StringAddr](std::byte* Address) -> bool {
            if (!Is32BitRelativeAddress(*(uint8_t*)(Address + 2)))
                return false;

            if ((std::byte*)CalculateRVA((uintptr_t)Address, 3) != StringAddr)
                return false;

            return true;
            };

        return ItterateAll(hat::compile_signature<"48 8D ? ? ? ? ?">(), ".text", Validate);
    }

    template<int Alignement = 4, typename T>
    inline int32_t FindOffset(std::vector<std::pair<void*, T>>& ObjectValuePair, int MinOffset = 0x28, int MaxOffset = 0x1A0)
    {
        int32_t HighestFoundOffset = MinOffset;

        for (int i = 0; i < ObjectValuePair.size(); i++)
        {
            uint8_t* BytePtr = (uint8_t*)(ObjectValuePair[i].first);

            for (int j = HighestFoundOffset; j < MaxOffset; j += Alignement)
            {
                if ((*reinterpret_cast<T*>(BytePtr + j)) == ObjectValuePair[i].second && j >= HighestFoundOffset)
                {
                    if (j > HighestFoundOffset)
                    {
                        HighestFoundOffset = j;
                        i = 0;
                    }
                    j = MaxOffset;
                }
            }
        }

        return HighestFoundOffset != MinOffset ? HighestFoundOffset : OFFSET_NOT_FOUND;
    }

    template<bool bCheckForVft = true>
    inline int32_t GetValidPointerOffset(uint8_t* ObjA, uint8_t* ObjB, int32_t StartingOffset, int32_t MaxOffset)
    {
        auto IsBadReadPtr = [](void* p) -> bool {
            MEMORY_BASIC_INFORMATION mbi;

            if (VirtualQuery(p, &mbi, sizeof(mbi))) {
                constexpr DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
                bool b = !(mbi.Protect & mask);
                if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
                    b = true;

                return b;
            }

            return true;
            };

        if (IsBadReadPtr(ObjA) || IsBadReadPtr(ObjB))
            return OFFSET_NOT_FOUND;

        for (int j = StartingOffset; j <= MaxOffset; j += 0x8)
        {
            const bool bIsAValid = !IsBadReadPtr(*reinterpret_cast<void**>(ObjA + j)) && (bCheckForVft ? !IsBadReadPtr(**reinterpret_cast<void***>(ObjA + j)) : true);
            const bool bIsBValid = !IsBadReadPtr(*reinterpret_cast<void**>(ObjB + j)) && (bCheckForVft ? !IsBadReadPtr(**reinterpret_cast<void***>(ObjB + j)) : true);

            if (bIsAValid && bIsBValid)
                return j;
        }

        return OFFSET_NOT_FOUND;
    };
}