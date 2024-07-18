#pragma once
#include <vector>
#include <libhat.hpp>

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
}