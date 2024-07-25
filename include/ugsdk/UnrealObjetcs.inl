#pragma once
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
	template<typename ReturnType, typename... Args>
	ReturnType UObject::Call(const UFunction* Function, Args&&... args)
	{
		constexpr uint8_t NumArgs = sizeof...(Args);
		constexpr uint16_t ArgsSize = (sizeof(Args) + ...);

		static uint16_t ArgOffsets[NumArgs] = { 0 };

		static size_t ParmsSize = 0;
		static uint16_t ReturnValueOffset = 0;
		static bool HasReturnValue = false;

		static bool Setup = false;
		if (!Setup)
		{
			ParmsSize = Function->ParmsSize();
			ReturnValueOffset = Function->ReturnValueOffset();
			HasReturnValue = ReturnValueOffset == UINT16_MAX;

			int i = 0;
			for (SDK::UField* Child = Function->Children(); Child; Child = Child->Next())
			{
				if (!Child->HasTypeFlag(CASTCLASS_FProperty))
					continue;

				SDK::UProperty* Property = static_cast<SDK::UProperty*>(Child);
				if (Property->HasPropertyFlag(CPF_ReturnParm))
					continue;

				ArgOffsets[i] = Property->Offset();
				i++;
			}

			Setup = true;
		}

		uint8_t* Parms = static_cast<uint8_t*>(alloca(ParmsSize));
		memset(Parms, 0, ParmsSize);

		auto AssignArg = [Parms](auto& Arg, uint16_t Offset) {
			using ArgType = std::decay_t<decltype(Arg)>;
			std::memcpy(Parms + Offset, &Arg, sizeof(ArgType));
			};

		int Index = 0;
		(AssignArg(args, ArgOffsets[Index++]), ...);

		ProcessEvent(Function, Parms);

		if (HasReturnValue)
		{
			ReturnType Return;
			std::memcpy(&Return, Parms + ReturnValueOffset, sizeof(ReturnType));
			return Return;
		}

		return ReturnType();
	}
}