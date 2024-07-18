#include <private/Memory.hpp>

namespace SDK::Memory
{
    uintptr_t CalculateRVA(uintptr_t Addr, uint32_t Offset)
    {
        return ((Addr + Offset + 4) + *(int32_t*)(Addr + Offset));
    }

    std::vector<std::byte*> FindAll(hat::signature_view Signature) {
        std::vector<std::byte*> Results = {};

        uintptr_t Start = (uintptr_t)hat::process::get_process_module();
        std::span<std::byte> Data = hat::process::get_module_data(hat::process::module_t(Start));
        if (Data.empty())
            return Results;

        const std::byte* DataBegin = Data.data();
        const std::byte* DataEnd = DataBegin + Data.size();

        while (DataBegin < DataEnd)
        {
            auto Result = hat::find_pattern(DataBegin, DataEnd, Signature);
            if (!Result.has_result())
                break;

            std::byte* ResPtr = (std::byte*)&(*Result.get());
            Results.emplace_back(ResPtr);
            DataBegin = ResPtr + 1;
        }

        return Results;
    }
    std::byte* FindPatternInRange(const std::byte* Start, const std::byte* End, hat::signature_view Signature)
    {
        auto Result = hat::find_pattern(Start, End, Signature);
        return (std::byte*)Result.get();
    }
}