#pragma once
#include <ugsdk/UnrealEnums.hpp>
#include <ugsdk/UnrealTypes.hpp>
#include <vector>

namespace SDK
{
    enum FSType
    {
        FS_UOBJECT,
        FS_UPROPERTY,
        FS_UFUNCTION,
    };

    struct FSUObject
    {
        FName ObjectName;
        EClassCastFlags RequiredType;
        class UObject** OutObject;

        template <typename T>
        explicit FSUObject(const std::string& ObjectName, uint64_t RequiredType, T** OutObject)
            : ObjectName(FName(ObjectName))
            , RequiredType((EClassCastFlags)(RequiredType))
            , OutObject(reinterpret_cast<class UObject**>(OutObject))
        {
        }
        template <typename T>
        explicit FSUObject(const std::string& ObjectName, T** OutObject)
            : ObjectName(FName(ObjectName))
            , RequiredType(CASTCLASS_None)
            , OutObject(reinterpret_cast<class UObject**>(OutObject))
        {
        }
    };
    struct FSUProperty
    {
        FName ClassName;
        FName PropertyName;
        uint32_t* OutOffset;
        uint8_t* OutMask;

        explicit FSUProperty(const std::string& ClassName, const std::string& PropertyName, uint32_t* OutOffset, uint8_t* OutMask)
            : ClassName(FName(ClassName))
            , PropertyName(FName(PropertyName))
            , OutOffset(OutOffset)
            , OutMask(OutMask)
        {
        }
    };
    struct FSUFunction
    {
        FName ClassName;
        FName FunctionName;
        class UFunction** OutFunction;

        explicit FSUFunction(const std::string& ClassName, const std::string& FunctionName, class UFunction** OutFunction)
            : ClassName(FName(ClassName))
            , FunctionName(FName(FunctionName))
            , OutFunction(OutFunction)
        {
        }
    };

    inline FSUObject FSUClass(const std::string& ObjectName, class UClass** OutObject) { return FSUObject(ObjectName, CASTCLASS_UClass, OutObject); }

    struct FSEntry
    {
        FSType Type;
        union
        {
            FSUObject Object;
            FSUProperty Property;
            FSUFunction Function;
        };

        FSEntry(const FSUObject& Object)
            : Type(FS_UOBJECT)
            , Object(Object)
        {
        }
        FSEntry(const FSUProperty& Property)
            : Type(FS_UPROPERTY)
            , Property(Property)
        {
        }
        FSEntry(const FSUFunction& Function)
            : Type(FS_UFUNCTION)
            , Function(Function)
        {
        }
    };
}

namespace SDK
{
    bool FastSearch(std::vector<FSEntry>& SearchList);

    template <typename T>
    bool FastSearch(const T Entry)
    {
        std::vector<FSEntry> Search = { Entry };
        return FastSearch(Search);
    }
}