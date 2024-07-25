#pragma once
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
	template<typename ReturnType, typename... Args>
	ReturnType Call(const UFunction* Function, Args&&... args)
	{
		constexpr size_t NumArgs = sizeof...(Args);
		constexpr size_t ArgsSize = (sizeof(Args) + ...);

		static int32_t ArgOffsets[NumArgs] = { 0 };	// Array of offsets into the params struct.

		static int32_t ReturnValueOffset = 0;		// Offset into params struct for the return value.
		static size_t ReturnValueSize = 0;			// The size of the return value.

		static size_t TotalParamsSize = 0;			// The size of all parameters added up.
		static size_t ParamsStructSize = 0;			// The size of all parameters added up + any padding.
		static size_t AlignedParamsStructSize = 0;	// The size of all parameters added up + any padding + final alignment.

		static bool Setup = false;
		if (!Setup)
		{
			size_t NumUFuncArgs = 0;				// The number of arguments found in the UFunction. (Excluding the return value)
			for (SDK::UField* Child = Function->Children(); Child; Child = Child->Next())
			{
				if (!Child->HasTypeFlag(CASTCLASS_FProperty))
					continue;

				SDK::UProperty* Property = static_cast<SDK::UProperty*>(Child);
				if (Property->HasPropertyFlag(CPF_ReturnParm))
				{
					ReturnValueOffset = Property->Offset();
					ReturnValueSize = Property->ElementSize();
					continue;
				}

				// If we reach here the property is a parameter and not the return parameter.
				if (NumUFuncArgs >= NumArgs)
					throw std::runtime_error("ProcessEvent argument count too small!");

				ArgOffsets[NumUFuncArgs] = Property->Offset();
				TotalParamsSize += Property->ElementSize();
				ParamsStructSize = Property->Offset() + Property->ElementSize();
				AlignedParamsStructSize = ParamsStructSize + 16; // I am yet to setup proper alignment detection, so a static offset of 16 will have to do.

				NumUFuncArgs++;
			}

			if (NumArgs < NumUFuncArgs)
				throw std::runtime_error(NumUFuncArgs ? "ProcessEvent argument count too large!" : "ProcessEvent function takes no arguments!");
		}
	}
}