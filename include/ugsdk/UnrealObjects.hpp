#pragma once
#include <ugsdk/Macros.hpp>
#include <ugsdk/UnrealEnums.hpp>
#include <memory>
#include <string>

namespace SDK
{
    struct PropertyInfo
    {
        bool Found = false;
        int32_t Flags;
        int32_t Offset;
        uint8_t ByteMask;
    };
}

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
        DECLARE_GETTER_SETTER(class FName, Name);
        DECLARE_GETTER_SETTER(class UObject*, Outer);

    public:
        bool HasTypeFlag(EClassCastFlags TypeFlag);
        bool IsA(class UClass* Target);
        bool IsDefaultObject();

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

    public:
        UField* FindMember(const std::string& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
        UField* FindMember(const FName& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
        PropertyInfo FindProperty(const std::string& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
        PropertyInfo FindProperty(const FName& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
        class UFunction* FindFunction(const std::string& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
        class UFunction* FindFunction(const FName& Name, EClassCastFlags TypeFlag = CASTCLASS_None);
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

    class UProperty : public UField
    {
    private:
        UProperty() = delete;
        ~UProperty() = delete;

    public:
        DECLARE_GETTER_SETTER(int32_t, Offset);
        DECLARE_GETTER_SETTER(int32_t, ElementSize);
    };

    class UBoolProperty : public UProperty
    {
    private:
        UBoolProperty() = delete;
        ~UBoolProperty() = delete;

    public:
        bool IsNativeBool();
        uint8_t GetFieldMask();
        uint8_t GetBitIndex();
    };

    class UFunction : public UStruct
    {
    private:
        UFunction() = delete;
        ~UFunction() = delete;
    };
}