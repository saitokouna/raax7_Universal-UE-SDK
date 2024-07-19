#pragma once
#include <ugsdk/Macros.hpp>
#include <ugsdk/UnrealEnums.hpp>
#include <memory>
#include <string>

namespace SDK
{
    class UObject
    {
    private:
        UObject() = delete;
        ~UObject() = delete;

    public:
        DECLARE_GETTER_SETTER(void**, VFT);
        DECLARE_GETTER_SETTER(int32_t, Flags);
        DECLARE_GETTER_SETTER(int32_t, Index);
        DECLARE_GETTER_SETTER(class UClass*, Class);
        DECLARE_GETTER_SETTER(class FName*, Name);
        DECLARE_GETTER_SETTER(class UObject*, Outer);

    public:
        bool HasTypeFlag(EClassCastFlags TypeFlag);
        bool IsA(class UClass* Target);

        std::string GetName();
        std::string GetFullName();
    };

    class UField : public UObject
    {
    private:
        UField() = delete;
        ~UField() = delete;

    public:
        DECLARE_GETTER_SETTER(class UField*, Next);
    };

    class UStruct : public UField
    {
    private:
        UStruct() = delete;
        ~UStruct() = delete;

    public:
        DECLARE_GETTER_SETTER(class UStruct*, Super);
        DECLARE_GETTER_SETTER(class UField*, Children);
        //DECLARE_GETTER_SETTER(FField*, ChildProperties);
    };

    class UClass : public UStruct
    {
    private:
        UClass() = delete;
        ~UClass() = delete;

    public:
        DECLARE_GETTER_SETTER(EClassCastFlags, CastFlags);
        DECLARE_GETTER_SETTER(class UObject*, DefaultObject);
    };

    class UProperty : public UObject
    {
    private:
        UProperty() = delete;
        ~UProperty() = delete;

    public:
        DECLARE_GETTER_SETTER(int32_t, Offset);
    };

    class UBoolProperty : public UProperty
    {
    private:
        UBoolProperty() = delete;
        ~UBoolProperty() = delete;

    public:
        DECLARE_GETTER_SETTER(uint8_t, ByteMask);
    };
}