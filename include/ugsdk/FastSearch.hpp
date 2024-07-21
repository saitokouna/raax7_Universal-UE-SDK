#pragma once
#include <ugsdk/UnrealObjects.hpp>
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
		uint64_t TargetClassFlags;
		UObject** OutObject;

		explicit FSUObject(const std::string& ObjectName, uint64_t TargetClassFlags, UObject** OutObject)
			: ObjectName(FName(ObjectName)), TargetClassFlags(TargetClassFlags), OutObject(OutObject) {}
	};
	struct FSUProperty
	{
		FName ClassName;
		FName PropertyName;
		uint32_t* OutOffset;
		uint8_t* OutMask;

		explicit FSUProperty(const std::string& ClassName, const std::string& PropertyName, uint32_t* OutOffset, uint8_t* OutMask)
			: ClassName(FName(ClassName)), PropertyName(FName(PropertyName)), OutOffset(OutOffset), OutMask(OutMask) {}
	};
	struct FSUFunction
	{
		FName ClassName;
		FName FunctionName;
		int32_t TargetFlags;
		UFunction** OutFunction;

		explicit FSUFunction(const std::string& ClassName, const std::string& FunctionName, UFunction** OutFunction)
			: ClassName(FName(ClassName)), FunctionName(FName(FunctionName)), OutFunction(OutFunction) {}
	};

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
			: Type(FS_UOBJECT), Object(Object) {}
		FSEntry(const FSUProperty& Property)
			: Type(FS_UPROPERTY), Property(Property) {}
		FSEntry(const FSUFunction& Function)
			: Type(FS_UFUNCTION), Function(Function) {}
	};
}

namespace SDK
{
	bool FastSearch(std::vector<FSEntry>& SearchList);
}