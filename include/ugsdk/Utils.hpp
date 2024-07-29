#pragma once

#include <algorithm>
#include <cstdint>

namespace SDK
{
    template <size_t N>
    struct ConstString
    {
        char value[N];

        constexpr ConstString(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }

        constexpr const char* c_str() const
        {
            return value;
        }
    };
}