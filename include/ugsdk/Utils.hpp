#pragma once

#include <algorithm>
#include <cstdint>

namespace SDK
{
    template <size_t N>
    struct StringLiteral
    {
        char String[N];

        consteval StringLiteral(const char (&str)[N])
        {
            std::copy_n(str, N, String);
        }

        consteval const char* c_str() const
        {
            return String;
        }
    };
}