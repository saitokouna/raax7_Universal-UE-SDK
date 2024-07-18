#pragma once
#include <ugsdk/Macros.hpp>
#include <ugsdk/ObjectArray.hpp>
#include <memory>

namespace SDK
{
    extern class FName;
    extern class TUObjectArray;
    class UClass;

    class UObject
    {
    private:
        UObject() = delete;
        ~UObject() = delete;

    public:
        static inline std::unique_ptr<TUObjectArray> Objects = nullptr;

    public:
        DECLARE_GETTER_SETTER(void**, VFT);
        DECLARE_GETTER_SETTER(int32_t, Flags);
        DECLARE_GETTER_SETTER(int32_t, Index);
        DECLARE_GETTER_SETTER(UClass*, Class);
        DECLARE_GETTER_SETTER(FName*, Name);
        DECLARE_GETTER_SETTER(UObject*, Outer);
    };

    class UField : public UObject
    {
    private:
        UField() = delete;
        ~UField() = delete;

    public:
        DECLARE_GETTER_SETTER(UField*, Next);
    };

    class UStruct : public UField
    {
    private:
        UStruct() = delete;
        ~UStruct() = delete;

    public:
        DECLARE_GETTER_SETTER(UStruct*, Super);
        DECLARE_GETTER_SETTER(UField*, Children);
        //DECLARE_GETTER_SETTER(FField*, ChildProperties);
    };

    class UClass : public UStruct
    {
    private:
        UClass() = delete;
        ~UClass() = delete;

    public:
        //DECLARE_GETTER_SETTER(EClassCastFlags, CastFlags);
        DECLARE_GETTER_SETTER(UObject*, DefaultObject);
    };
}