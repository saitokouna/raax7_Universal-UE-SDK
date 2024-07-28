#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <ugsdk/Macros.hpp>
#include <ugsdk/UnrealEnums.hpp>
#include <ugsdk/Utils.hpp>

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

    private:
        struct ArgInfo
        {
            int32_t Offset;
            int32_t Size;
            bool IsOutParm;
        };

    public:
        void** VFT;
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

        void ProcessEvent(class UFunction* Function, void* Parms);

        /**
         * @brief Calls a ProcessEvent function, automatically handling the parameter structure.
         * @brief This function requires the following:
         * @brief - All arguments must match the order of the UFunction.
         * @brief - Output arguments are only supported using pointers, not references.
         * @brief - Pointers to output arguments can be larger than the actual struct; only the real size of the struct will be copied.
         *
         * @param Function - A pointer to the UFunction. If it is nullptr, the UFunction will be found using the ClassName and FunctionName.
         * @param ReturnType - The return type of the function, void by default.
         * @tparam ClassName and FunctionName - Used to force each template instance to be unique. If no UFunction is provided, it is used to find the target function.
         * @tparam ReturnType - The return type of the function, void by default.
         * @tparam Args - Types of the arguments to be sent to the UFunction.
         *
         * @return The result of the UFunction call, if there is a return type.
         *
         * @throws std::invalid_argument - If no Function pointer was passed and finding the function failed.
         * @throws std::invalid_argument - If an output argument is not a pointer.
         * @throws std::bad_alloc - If allocating memory for the parameters on the stack fails.
         * @throws std::logic_error - If a return type was specified, but the UFunction does not have a return type.
         */
        template <ConstString ClassName, ConstString FunctionName, typename ReturnType = void, typename... Args>
        ReturnType Call(class UFunction** Function = nullptr, Args&&... args);

    private:
        template <int N>
        void InitializeArgInfo(UFunction* Function, std::array<ArgInfo, N>& ArgOffsets, size_t& ParmsSize, int32_t& ReturnValueOffset, int32_t& ReturnValueSize, bool& HasReturnValue);
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
        // DECLARE_GETTER_SETTER(FField*, ChildProperties);

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
        bool HasPropertyFlag(EPropertyFlags PropertyFlag);

    public:
        DECLARE_GETTER_SETTER(int32_t, Offset);
        DECLARE_GETTER_SETTER(int32_t, ElementSize);
        DECLARE_GETTER_SETTER(EPropertyFlags, PropertyFlags);
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

    public:
        using FNativeFuncPtr = void (*)(void* Context, void* TheStack, void* Result);

    public:
        DECLARE_GETTER_SETTER(EFunctionFlags, FunctionFlags);
        DECLARE_GETTER_SETTER(uint8_t, NumParms);
        DECLARE_GETTER_SETTER(uint16_t, ParmsSize);
        DECLARE_GETTER_SETTER(uint16_t, ReturnValueOffset);
        DECLARE_GETTER_SETTER(FNativeFuncPtr, Func);
    };
}

#include <ugsdk/UnrealObjetcs.inl>