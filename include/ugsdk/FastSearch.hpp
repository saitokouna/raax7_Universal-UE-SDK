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
		UObject** OutObject;
	};
	struct FSUProperty
	{
		FName ClassName;
		FName PropertyName;
		uint32_t* OutOffset;
		uint8_t* OutMask;
	};
	struct FSUFunction
	{
		FName ClassName;
		FName FunctionName;
		UFunction** OutFunction;
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
	};
}

namespace SDK
{
	bool FastSearch(const std::vector<FSEntry>& SearchList);
}