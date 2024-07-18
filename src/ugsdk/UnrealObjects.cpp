#pragma once
#include <ugsdk/UnrealObjects.hpp>
#include <ugsdk/UnrealTypes.hpp>
#include <private/Offsets.hpp>

namespace SDK
{
	DEFINE_GETTER_SETTER(UObject, void**, VFT, SDK::Offsets::UObject::VFT);
	DEFINE_GETTER_SETTER(UObject, int32_t, Flags, SDK::Offsets::UObject::Flags);
	DEFINE_GETTER_SETTER(UObject, int32_t, Index, SDK::Offsets::UObject::Index);
	DEFINE_GETTER_SETTER(UObject, UClass*, Class, SDK::Offsets::UObject::Class);
	FName* UObject::Name()
	{
		return (FName*)((uintptr_t)this + SDK::Offsets::UObject::Name);
	}
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
	std::string UObject::GetName()
	{
		return this ? Name()->ToString() : "None";
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

	DEFINE_GETTER_SETTER(UStruct, UStruct*, Super, SDK::Offsets::UStruct::Super);
	DEFINE_GETTER_SETTER(UStruct, UField*, Children, SDK::Offsets::UStruct::Children);

	DEFINE_GETTER_SETTER(UClass, EClassCastFlags, CastFlags, SDK::Offsets::UClass::CastFlags);
	DEFINE_GETTER_SETTER(UClass, UObject*, DefaultObject, SDK::Offsets::UClass::DefaultObject);
}