#pragma once
#include <private/Offsets.hpp>
#include <ugsdk/Settings.hpp>
#include <ugsdk/UnrealContainers.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
    DEFINE_GETTER_SETTER(UObject, int32_t, Flags, SDK::Offsets::UObject::Flags);
    DEFINE_GETTER_SETTER(UObject, int32_t, Index, SDK::Offsets::UObject::Index);
    DEFINE_GETTER_SETTER(UObject, UClass*, Class, SDK::Offsets::UObject::Class);
    DEFINE_GETTER_SETTER(UObject, FName, Name, SDK::Offsets::UObject::Name);
    DEFINE_GETTER_SETTER(UObject, UObject*, Outer, SDK::Offsets::UObject::Outer);
    bool UObject::HasTypeFlag(EClassCastFlags TypeFlag)
    {
        return TypeFlag != CASTCLASS_None ? Class()->ClassCastFlags() & TypeFlag : true;
    }
    bool UObject::IsA(UClass* Target)
    {
        for (UStruct* Super = Class(); Super; Super = Super->SuperStruct()) {
            if (Super == Target) {
                return true;
            }
        }

        return false;
    }
    bool UObject::IsDefaultObject()
    {
        return ((Flags() & 0x10) == 0x10);
    }
    std::string UObject::GetName()
    {
        return this ? Name().ToString() : "None";
    }
    std::string UObject::GetFullName()
    {
        if (this && Class()) {
            std::string Temp;

            for (UObject* NextOuter = Outer(); NextOuter; NextOuter = NextOuter->Outer()) {
                Temp = NextOuter->GetName() + "." + Temp;
            }

            std::string Name = Class()->GetName();
            Name += " ";
            Name += Temp;
            Name += GetName();

            return Name;
        }

        return "None";
    }
    void UObject::ProcessEventAsNative(class UFunction* Function, void* Parms)
    {
        EFunctionFlags FunctionFlags = Function->FunctionFlags();
        Function->FunctionFlags(FunctionFlags | FUNC_Native);
        ProcessEvent(Function, Parms);
        Function->FunctionFlags(FunctionFlags);
    }
    void UObject::ProcessEvent(UFunction* Function, void* Parms)
    {
        using ProcessEvent_t = void (*)(UObject*, UFunction*, void*);
        ProcessEvent_t PE = reinterpret_cast<ProcessEvent_t>(VFT[Offsets::UObject::ProcessEventIdx]);
        PE(this, Function, Parms);
    }

    DEFINE_GETTER_SETTER(UField, UField*, Next, SDK::Offsets::UField::Next);
    UField* UStruct::FindMember(const FName& Name, EClassCastFlags TypeFlag)
    {
        for (SDK::UField* Child = Children(); Child; Child = Child->Next()) {
            if (Child->HasTypeFlag(TypeFlag) && Child->Name() == Name)
                return Child;
        }

        return nullptr;
    }
    PropertyInfo UStruct::FindProperty(const FName& Name, EClassCastFlags TypeFlag)
    {
        PropertyInfo Result = {};
        Result.Found = false;

        if (SDK::Settings::UsesFProperty) {
            UObject* DefaultClass = Class()->ClassDefaultObject();
            if (!DefaultClass || !DefaultClass->HasTypeFlag(CASTCLASS_UClass))
                return Result;

            for (FField* Field = reinterpret_cast<UClass*>(DefaultClass)->ChildProperties(); Field; Field = Field->Next) {
                if (!Field->HasTypeFlag(CASTCLASS_FProperty))
                    continue;

                if (Field->Name == Name) {
                    FProperty* Property = reinterpret_cast<FProperty*>(Field);

                    Result.Found = true;
                    Result.Flags = Property->PropertyFlags;
                    Result.Offset = Property->Offset;
                    if (Property->HasTypeFlag(CASTCLASS_FBoolProperty) && !reinterpret_cast<FBoolProperty*>(Property)->IsNativeBool())
                        Result.ByteMask = reinterpret_cast<UBoolProperty*>(Property)->GetFieldMask();

                    return Result;
                }
            }
        }
        else {
            for (UField* Child = Children(); Child; Child = Child->Next()) {
                if (!Child->HasTypeFlag(CASTCLASS_FProperty) && !Child->HasTypeFlag(TypeFlag))
                    continue;

                if (Child->Name() == Name) {
                    UProperty* Property = reinterpret_cast<UProperty*>(Child);

                    Result.Found = true;
                    Result.Flags = Child->Flags();
                    Result.Offset = Property->Offset();
                    if (Property->HasTypeFlag(CASTCLASS_FBoolProperty) && !reinterpret_cast<UBoolProperty*>(Property)->IsNativeBool())
                        Result.ByteMask = reinterpret_cast<UBoolProperty*>(Property)->GetFieldMask();

                    return Result;
                }
            }
        }

        return Result;
    }
    UFunction* UStruct::FindFunction(const FName& Name)
    {
        return reinterpret_cast<UFunction*>(FindMember(Name, CASTCLASS_UFunction));
    }

    DEFINE_GETTER_SETTER(UStruct, UStruct*, SuperStruct, SDK::Offsets::UStruct::SuperStruct);
    DEFINE_GETTER_SETTER(UStruct, UField*, Children, SDK::Offsets::UStruct::Children);
    DEFINE_GETTER_SETTER(UStruct, FField*, ChildProperties, SDK::Offsets::UStruct::ChildProperties);
    DEFINE_GETTER_SETTER(UStruct, int32_t, PropertiesSize, SDK::Offsets::UStruct::PropertiesSize);
    DEFINE_GETTER_SETTER(UStruct, int32_t, MinAlignment, SDK::Offsets::UStruct::MinAlignment);

    DEFINE_GETTER_SETTER(UClass, EClassCastFlags, ClassCastFlags, SDK::Offsets::UClass::ClassCastFlags);
    DEFINE_GETTER_SETTER(UClass, UObject*, ClassDefaultObject, SDK::Offsets::UClass::ClassDefaultObject);

    bool UProperty::HasPropertyFlag(EPropertyFlags PropertyFlag)
    {
        return PropertyFlag != CPF_None ? PropertyFlags() & PropertyFlag : true;
    }
    DEFINE_GETTER_SETTER(UProperty, int32_t, Offset, SDK::Offsets::UProperty::Offset);
    DEFINE_GETTER_SETTER(UProperty, int32_t, ElementSize, SDK::Offsets::UProperty::ElementSize);
    DEFINE_GETTER_SETTER(UProperty, EPropertyFlags, PropertyFlags, SDK::Offsets::UProperty::PropertyFlags);

    bool UBoolProperty::IsNativeBool()
    {
        return GetFieldMask() == 0xFF;
    }
    uint8_t UBoolProperty::GetFieldMask()
    {
        return reinterpret_cast<SDK::Offsets::UBoolProperty::UBoolPropertyBase*>((uintptr_t)this + SDK::Offsets::UBoolProperty::Base)->FieldMask;
    }
    uint8_t UBoolProperty::GetBitIndex()
    {
        uint8_t FieldMask = GetFieldMask();

        if (FieldMask != 0xFF) {
            if (FieldMask == 0x01) {
                return 0;
            }
            if (FieldMask == 0x02) {
                return 1;
            }
            if (FieldMask == 0x04) {
                return 2;
            }
            if (FieldMask == 0x08) {
                return 3;
            }
            if (FieldMask == 0x10) {
                return 4;
            }
            if (FieldMask == 0x20) {
                return 5;
            }
            if (FieldMask == 0x40) {
                return 6;
            }
            if (FieldMask == 0x80) {
                return 7;
            }
        }

        return 0xFF;
    }

    TArray<TPair<FName, int64_t>> UEnum::Names() const
    {
        return *(TArray<TPair<FName, int64_t>>*)((uintptr_t)this + SDK::Offsets::UEnum::Names);
    }
    int64_t UEnum::FindEnumerator(const FName& Name)
    {
        auto NamesArray = Names();
        for (auto& It : NamesArray) {
            if (It.Key() == Name) {
                return It.Value();
            }
        }

        return OFFSET_NOT_FOUND;
    }

    DEFINE_GETTER_SETTER(UFunction, EFunctionFlags, FunctionFlags, SDK::Offsets::UFunction::FunctionFlags);
    DEFINE_GETTER_SETTER(UFunction, uint8_t, NumParms, SDK::Offsets::UFunction::NumParms);
    DEFINE_GETTER_SETTER(UFunction, uint16_t, ParmsSize, SDK::Offsets::UFunction::ParmsSize);
    DEFINE_GETTER_SETTER(UFunction, uint16_t, ReturnValueOffset, SDK::Offsets::UFunction::ReturnValueOffset);
    DEFINE_GETTER_SETTER(UFunction, UFunction::FNativeFuncPtr, Func, SDK::Offsets::UFunction::FuncOffset);
}