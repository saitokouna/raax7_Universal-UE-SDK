#include <private/Memory.hpp>

namespace SDK::Memory
{
    uintptr_t CalculateRVA(uintptr_t Addr, uint32_t Offset)
    {
        return ((Addr + Offset + 4) + *(int32_t*)(Addr + Offset));
    }
    bool Is32BitRelativeAddress(uint8_t ModRM)
    {
        // MOD - Bit 6-7
        // REG - Bit 3-5
        // R/M - Bit 0-2
        //
        // MOD should be 0b00 and R/M should be 0b101.
        // REG varies depending on the output register
        // so we can't check it.

        return ((ModRM & 0b11000111) == 0b00000101);
    }

    std::byte* ItterateAll(hat::signature_view Signature, const std::string& Section, std::function<bool(std::byte*)> It) {
        uintptr_t Start = (uintptr_t)hat::process::get_process_module();
        std::span<std::byte> Data = hat::process::get_section_data(hat::process::module_t(Start), Section);
        if (Data.empty())
            return nullptr;

        const std::byte* DataBegin = Data.data();
        const std::byte* DataEnd = DataBegin + Data.size();

        while (DataBegin < DataEnd)
        {
            auto Result = hat::find_pattern(DataBegin, DataEnd, Signature);
            if (!Result.has_result())
                break;

            std::byte* ResPtr = (std::byte*)&(*Result.get());
            if (It(ResPtr))
                return ResPtr;

            DataBegin = ResPtr + 1;
        }

        return nullptr;
    }
    std::byte* FindPatternInRange(const std::byte* Start, const std::byte* End, hat::signature_view Signature)
    {
        auto Result = hat::find_pattern(Start, End, Signature);
        return (std::byte*)Result.get();
    }
}