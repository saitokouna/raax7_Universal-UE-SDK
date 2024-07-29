#pragma once

#include <Windows.h>
#include <algorithm>
#include <array>
#include <format>
#include <ugsdk/FastSearch.hpp>
#include <ugsdk/Settings.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
    template <ConstString ClassName, ConstString FunctionName, typename ReturnType, typename... Args>
    ReturnType UObject::Call(UFunction* Function, Args&&... args)
    {
        constexpr size_t NumArgs = sizeof...(Args);
        static std::array<ArgInfo, (NumArgs > 0 ? NumArgs : 1)> ArgOffsets = { 0 };

        static size_t ParmsSize = 0;
        static int32_t ReturnValueOffset = 0;
        static int32_t ReturnValueSize = 0;
        static bool HasReturnValue = false;

        // If there are no arguments and no return type, we can simply call ProcessEvent.
        if constexpr (std::is_void_v<ReturnType> && !NumArgs) {
            ProcessEventAsNative(Function, nullptr);
            return;
        }

        static bool Setup = false;
        if (!Setup) {
            InitializeArgInfo(Function, ArgOffsets, ParmsSize, ReturnValueOffset, ReturnValueSize, HasReturnValue);
            Setup = true;
        }

        // Allocate memory on the stack for the parameters.
        uint8_t* Parms = static_cast<uint8_t*>(_malloca(ParmsSize));
        if (!Parms)
            throw std::bad_alloc();

        ZeroMemory(Parms, ParmsSize);

        // Write input arguments to the parameter buffer.
        {
            auto WriteInputArg = [Parms](auto& Arg, ArgInfo& Info) {
                using ArgType = std::decay_t<decltype(Arg)>;
                memcpy(Parms + Info.Offset, &Arg, std::min<int32_t>(Info.Size, sizeof(ArgType)));
            };

            int ArgIndex = 0;
            ((ArgOffsets[ArgIndex].IsOutParm ? void() : WriteInputArg(args, ArgOffsets[ArgIndex]), ++ArgIndex), ...);
        }

        ProcessEventAsNative(Function, Parms);

        // Write output arguments.
        {
            auto WriteOutputArg = [Parms](auto& Arg, ArgInfo& Info) {
                using ArgType = std::decay_t<decltype(Arg)>;

                if constexpr (!std::is_void_v<std::remove_pointer_t<ArgType>> && std::is_pointer_v<ArgType>) {
                    if (Arg)
                        std::memcpy(Arg, Parms + Info.Offset, sizeof(*Arg));
                    else
                        throw std::invalid_argument("Only pointer output parameters are supported!");
                }
            };

            int ArgIndex = 0;
            ((ArgOffsets[ArgIndex].IsOutParm ? WriteOutputArg(args, ArgOffsets[ArgIndex]) : void(), ++ArgIndex), ...);
        }

        if constexpr (!std::is_void_v<ReturnType>) {
            if (!HasReturnValue)
                throw std::logic_error("Function template specifices return type, but UFunction does not contain a return value!");

            ReturnType Return = {};
            memcpy(&Return, Parms + ReturnValueOffset, std::min<int32_t>(ReturnValueSize, sizeof(ReturnType)));
            return Return;
        }
    }

    template <ConstString ClassName, ConstString FunctionName, typename ReturnType, typename... Args>
    ReturnType UObject::Call(Args&&... args)
    {
        static UFunction* Function = nullptr;

        // If there is no function, search for it.
        if (!Function) {
            if (!FastSearchSingle(FSUFunction(ClassName.c_str(), FunctionName.c_str(), &Function))) {
                throw std::invalid_argument("Failed to automatically find UFunction!");
            }
        }

        return Call<ClassName, FunctionName, ReturnType, Args...>(Function, std::forward<Args>(args)...);
    }

    template <int N>
    void UObject::InitializeArgInfo(UFunction* Function, std::array<ArgInfo, N>& ArgOffsets, size_t& ParmsSize, int32_t& ReturnValueOffset, int32_t& ReturnValueSize, bool& HasReturnValue)
    {
        ParmsSize = Function->ParmsSize();
        ReturnValueOffset = Function->ReturnValueOffset();
        HasReturnValue = ReturnValueOffset != UINT16_MAX;

        int ArgIndex = 0;
        if (Settings::UsesFProperty) {
            for (FField* Field = Function->ChildProperties(); Field; Field = Field->Next) {
                if (!Field->HasTypeFlag(CASTCLASS_FProperty))
                    continue;

                FProperty* Property = static_cast<FProperty*>(Field);
                if (Property->HasPropertyFlag(CPF_ReturnParm)) {
                    if (Property->Offset == ReturnValueOffset)
                        ReturnValueSize = Property->ElementSize;

                    continue;
                }

                ArgOffsets[ArgIndex] = { Property->Offset, Property->ElementSize, Property->HasPropertyFlag(CPF_OutParm) };
                ArgIndex++;
            }
        }
        else {
            for (UField* Child = Function->Children(); Child; Child = Child->Next()) {
                if (!Child->HasTypeFlag(CASTCLASS_FProperty))
                    continue;

                UProperty* Property = static_cast<UProperty*>(Child);
                if (Property->HasPropertyFlag(CPF_ReturnParm)) {
                    if (Property->Offset() == ReturnValueOffset)
                        ReturnValueSize = Property->ElementSize();

                    continue;
                }

                ArgOffsets[ArgIndex] = { Property->Offset(), Property->ElementSize(), Property->HasPropertyFlag(CPF_OutParm) };
                ArgIndex++;
            }
        }
    }
}