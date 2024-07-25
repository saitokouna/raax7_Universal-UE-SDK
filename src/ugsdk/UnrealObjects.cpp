#pragma once
#include <ugsdk/UnrealObjects.hpp>
#include <ugsdk/UnrealTypes.hpp>
#include <private/Offsets.hpp>
#include <private/Settings.hpp>

namespace SDK
{
	DEFINE_GETTER_SETTER(UObject, void**, VFT, SDK::Offsets::UObject::VFT);
	DEFINE_GETTER_SETTER(UObject, int32_t, Flags, SDK::Offsets::UObject::Flags);
	DEFINE_GETTER_SETTER(UObject, int32_t, Index, SDK::Offsets::UObject::Index);
	DEFINE_GETTER_SETTER(UObject, UClass*, Class, SDK::Offsets::UObject::Class);
	DEFINE_GETTER_SETTER(UObject, FName, Name, SDK::Offsets::UObject::Name);
	DEFINE_GETTER_SETTER(UObject, UObject*, Outer, SDK::Offsets::UObject::Outer);
	bool UObject::HasTypeFlag(EClassCastFlags TypeFlag)
	{
		return TypeFlag != CASTCLASS_None ? Class()->CastFlags() & TypeFlag : true;
	}
	bool UObject::IsA(UClass* Target)
	{
		for (UStruct* Super = Class(); Super; Super = Super->Super())
		{
			if (Super == Target)
			{
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
		if (this && Class())
		{
			std::string Temp;

			for (UObject* NextOuter = Outer(); NextOuter; NextOuter = NextOuter->Outer())
			{
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

	DEFINE_GETTER_SETTER(UField, UField*, Next, SDK::Offsets::UField::Next);
	UField* UStruct::FindMember(const std::string& Name, EClassCastFlags TypeFlag)
	{
		return FindMember(FName(Name), TypeFlag);
	}
	UField* UStruct::FindMember(const FName& Name, EClassCastFlags TypeFlag)
	{
		for (SDK::UField* Child = Children(); Child; Child = Child->Next())
		{
			if (Child->HasTypeFlag(TypeFlag) && Child->Name() == Name)
				return Child;
		}

		return nullptr;
	}
	PropertyInfo UStruct::FindProperty(const std::string& Name, EClassCastFlags TypeFlag)
	{
		return FindProperty(FName(Name), TypeFlag);
	}
	PropertyInfo UStruct::FindProperty(const FName& Name, EClassCastFlags TypeFlag)
	{
		PropertyInfo Result = {};
		Result.Found = false;

		if (SDK::Settings::UsesFProperty)
		{

		}
		else
		{
			for (SDK::UField* Child = Children(); Child; Child = Child->Next())
			{
				if (!Child->HasTypeFlag(CASTCLASS_FProperty) && !Child->HasTypeFlag(TypeFlag))
					continue;

				if (Child->Name() == Name)
				{
					SDK::UProperty* Property = reinterpret_cast<UProperty*>(Child);

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
	UFunction* UStruct::FindFunction(const std::string& Name, EClassCastFlags TypeFlag)
	{
		return FindFunction(FName(Name), TypeFlag);
	}
	UFunction* UStruct::FindFunction(const FName& Name, EClassCastFlags TypeFlag)
	{
		for (SDK::UField* Child = Children(); Child; Child = Child->Next())
		{
			if (!Child->HasTypeFlag(CASTCLASS_UFunction) && !Child->HasTypeFlag(TypeFlag))
				continue;

			if (Child->Name() == Name)
				return reinterpret_cast<UFunction*>(Child);
		}

		return nullptr;
	}

	DEFINE_GETTER_SETTER(UStruct, UStruct*, Super, SDK::Offsets::UStruct::Super);
	DEFINE_GETTER_SETTER(UStruct, UField*, Children, SDK::Offsets::UStruct::Children);

	DEFINE_GETTER_SETTER(UClass, EClassCastFlags, CastFlags, SDK::Offsets::UClass::CastFlags);
	DEFINE_GETTER_SETTER(UClass, UObject*, DefaultObject, SDK::Offsets::UClass::DefaultObject);

	DEFINE_GETTER_SETTER(UProperty, int32_t, Offset, SDK::Offsets::UProperty::Offset);
	DEFINE_GETTER_SETTER(UProperty, int32_t, ElementSize, SDK::Offsets::UProperty::ElementSize);

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

		if (FieldMask != 0xFF)
		{
			if (FieldMask == 0x01) { return 0; }
			if (FieldMask == 0x02) { return 1; }
			if (FieldMask == 0x04) { return 2; }
			if (FieldMask == 0x08) { return 3; }
			if (FieldMask == 0x10) { return 4; }
			if (FieldMask == 0x20) { return 5; }
			if (FieldMask == 0x40) { return 6; }
			if (FieldMask == 0x80) { return 7; }
		}

		return 0xFF;
	}
}