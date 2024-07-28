#include <private/Offsets.hpp>
#include <string>
#include <ugsdk/UnrealTypes.hpp>

namespace SDK
{
    FName::FName(const std::string& Str)
        : ComparisonIdx(0)
        , Number(0)
    {
        static void (*Constructor)(const FName*, const char*, bool) = nullptr;

        if (!Constructor)
            Constructor = reinterpret_cast<void (*)(const FName*, const char*, bool)>(SDK::Offsets::FName::Constructor);

        Constructor(const_cast<SDK::FName*>(this), Str.c_str(), true);
    }

    std::string FName::GetRawString() const
    {
        static void (*AppendString)(const FName*, FString*) = nullptr;

        if (!AppendString)
            AppendString = reinterpret_cast<void (*)(const FName*, FString*)>(SDK::Offsets::FName::AppendString);

        FString TempString;
        AppendString(const_cast<SDK::FName*>(this), &TempString);

        return TempString.ToString();
    }
    std::string FName::ToString() const
    {
        std::string OutputString = GetRawString();
        size_t pos = OutputString.rfind('/');

        if (pos == std::string::npos)
            return OutputString;

        return OutputString.substr(pos + 1);
    }
}