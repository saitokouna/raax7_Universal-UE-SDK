#pragma once
#include <private/Offsets.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
	DEFINE_GETTER_SETTER(UObject, void**, VFT, SDK::Offsets::UObject::VFT);
	DEFINE_GETTER_SETTER(UObject, int32_t, Flags, SDK::Offsets::UObject::Flags);
	DEFINE_GETTER_SETTER(UObject, int32_t, Index, SDK::Offsets::UObject::Index);
	DEFINE_GETTER_SETTER(UObject, UClass*, Class, SDK::Offsets::UObject::Class);
	DEFINE_GETTER_SETTER(UObject, FName*, Name, SDK::Offsets::UObject::Name);
	DEFINE_GETTER_SETTER(UObject, UObject*, Outer, SDK::Offsets::UObject::Outer);

	DEFINE_GETTER_SETTER(UField, UField*, Next, SDK::Offsets::UField::Next);

	DEFINE_GETTER_SETTER(UStruct, UStruct*, Super, SDK::Offsets::UStruct::Super);
	DEFINE_GETTER_SETTER(UStruct, UField*, Children, SDK::Offsets::UStruct::Children);

	DEFINE_GETTER_SETTER(UClass, UObject*, DefaultObject, SDK::Offsets::UClass::DefaultObject);
}