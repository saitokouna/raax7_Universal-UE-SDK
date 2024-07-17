#pragma once
#include <Private/Offsets.hpp>
#include <Private/Macros.hpp>

namespace SDK
{
    class FName
    {
    private:
        FName() = delete;
        ~FName() = delete;

    public:
        uint32_t ComparisonIdx;
        uint32_t Number;
    };
}