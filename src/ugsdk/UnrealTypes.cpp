#include <ugsdk/UnrealTypes.hpp>
#include <private/Offsets.hpp>

namespace SDK
{
	std::string FName::GetRawString() const
	{
		if (!this || !SDK::Offsets::FName::AppendString)
			return "Failed!";

		static void(*AppendString)(const FName*, FString*) = nullptr;

		if (!AppendString)
			AppendString = reinterpret_cast<void(*)(const FName*, FString*)>(SDK::Offsets::FName::AppendString);

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