#pragma once
#include <Private/Offsets.hpp>
#include <Private/Macros.hpp>
#include <memory>

namespace SDK
{
    //extern class FName;
    class TUObjectArray;
    class UClass;

    class UObject
    {
    private:
        UObject() = delete;
        ~UObject() = delete;

    public:
        static inline std::unique_ptr<TUObjectArray> Objects = nullptr;

    public:
        CREATE_GETTER_SETTER(void**, VFT, SDK::Offsets::UObject::VFT);
        CREATE_GETTER_SETTER(int32_t, Flags, SDK::Offsets::UObject::Flags);
        CREATE_GETTER_SETTER(int32_t, Index, SDK::Offsets::UObject::Index);
        CREATE_GETTER_SETTER(UClass*, Class, SDK::Offsets::UObject::Class);
        //CREATE_GETTER_SETTER(FName, Name, SDK::Offsets::UObject::Name);
        CREATE_GETTER_SETTER(UObject*, Outer, SDK::Offsets::UObject::Outer);
    };

    class UField : public UObject
    {
    private:
        UField() = delete;
        ~UField() = delete;

    public:
        CREATE_GETTER_SETTER(UField*, Next, SDK::Offsets::UField::Next);
    };

    class UStruct : public UField
    {
    private:
        UStruct() = delete;
        ~UStruct() = delete;

    public:
        CREATE_GETTER_SETTER(UStruct*, Super, SDK::Offsets::UStruct::Super);
        CREATE_GETTER_SETTER(UField*, Children, SDK::Offsets::UStruct::Children);
        //CREATE_GETTER_SETTER(FField*, ChildProperties, SDK::Offsets::UStruct::ChildProperties);
    };

    class UClass : public UStruct
    {
    private:
        UClass() = delete;
        ~UClass() = delete;

    public:
        //CREATE_GETTER_SETTER(EClassCastFlags, CastFlags, SDK::Offsets::UClass::CastFlags);
        CREATE_GETTER_SETTER(UObject*, DefaultObject, SDK::Offsets::UClass::DefaultObject);
    };
}