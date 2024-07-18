#pragma once
#include <ugsdk/Macros.hpp>
#include <vector>
#include <libhat.hpp>

// Credit to https://github.com/Encryqed/Dumper-7 for the FindOffset function.

namespace SDK::Memory
{
    uintptr_t CalculateRVA(uintptr_t Addr, uint32_t Offset);

    std::vector<std::byte*> FindAll(hat::signature_view Signature);
    std::byte* FindPatternInRange(const std::byte* Start, const std::byte* End, hat::signature_view Signature);

    template<typename T>
    inline std::byte* FindString(const T* String) {
        const auto StringSig = hat::string_to_signature(std::basic_string_view<T>(String));
        return (std::byte*)hat::find_pattern(StringSig, ".rdata").get();
    }

    template<typename T>
    inline std::byte* FindStringRef(const T* String) {
        std::byte* StringAddr = FindString(String);

        auto ProcessPattern = [&StringAddr](const std::vector<std::byte*>& StringRefs) -> std::byte* {
            for (const auto& Found : StringRefs)
            {
                if ((std::byte*)CalculateRVA((uintptr_t)Found, 3) == StringAddr)
                    return Found;
            }

            return nullptr;
            };

        const std::vector<std::vector<std::byte*>> LEAs = {
            FindAll(hat::compile_signature<"48 8D 05 ? ? ? ?">()),
            FindAll(hat::compile_signature<"4C 8D 05 ? ? ? ?">())
        };

        for (const auto& Search : LEAs)
        {
            if (const auto Result = ProcessPattern(Search); Result)
                return Result;
        }

        return nullptr;
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
}