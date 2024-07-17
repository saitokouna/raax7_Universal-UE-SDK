#pragma once
#include <Private/Offsets.hpp>
#include <Private/Macros.hpp>

namespace SDK
{
    extern class FName;
    class UClass;

    class UObject
    {
    private:
        UObject() = delete;
        ~UObject() = delete;

    public:
        CREATE_GETTER_SETTER(void**, VFT, Offsets::UObject::VFT);
        CREATE_GETTER_SETTER(int32_t, Flags, Offsets::UObject::Flags);
        CREATE_GETTER_SETTER(int32_t, Index, Offsets::UObject::Index);
        CREATE_GETTER_SETTER(UClass*, Class, Offsets::UObject::Class);
        CREATE_GETTER_SETTER(FName, Name, Offsets::UObject::Name);
        CREATE_GETTER_SETTER(UObject*, Outer, Offsets::UObject::Outer);
    };

    class UField : public UObject
    {
    private:
        UField() = delete;
        ~UField() = delete;

    public:
        CREATE_GETTER_SETTER(UField*, Next, Offsets::UField::Next);
    };

    class UStruct : public UField
    {
    private:
        UStruct() = delete;
        ~UStruct() = delete;

    public:
        CREATE_GETTER_SETTER(UStruct*, Super, Offsets::UStruct::Super);
        CREATE_GETTER_SETTER(UField*, Children, Offsets::UStruct::Children);
        //CREATE_GETTER_SETTER(FField*, ChildProperties, Offsets::UStruct::ChildProperties);
    };

    class UClass : public UStruct
    {
    private:
        UClass() = delete;
        ~UClass() = delete;

    public:
        //CREATE_GETTER_SETTER(EClassCastFlags, CastFlags, Offsets::UClass::CastFlags);
        CREATE_GETTER_SETTER(UObject*, DefaultObject, Offsets::UClass::DefaultObject);
    };
}