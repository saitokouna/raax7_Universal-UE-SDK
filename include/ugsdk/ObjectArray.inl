#pragma once
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
	template<typename UEType>
	UEType* TUObjectArray::FindObject(const std::string& FullName, EClassCastFlags RequiredType)
	{
		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Object = GObjects->GetByIndex(i);
			if (!Object)
				continue;

			if (Object->HasTypeFlag(RequiredType) && Object->GetFullName() == FullName)
				return static_cast<UEType*>(Object);
		}

		return nullptr;
	}

	template<typename UEType>
	UEType* TUObjectArray::FindObjectFast(const std::string& Name, EClassCastFlags RequiredType)
	{
		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Object = GObjects->GetByIndex(i);
			if (!Object)
				continue;

			if (Object->HasTypeFlag(RequiredType) && Object->GetName() == Name)
				return static_cast<UEType*>(Object);
		}

		return nullptr;
	}

	template<typename UEType>
	UEType* TUObjectArray::FindObjectFastInOuter(const std::string& Name, const std::string& Outer)
	{
		for (int i = 0; i < GObjects->Num(); i++)
		{
			UObject* Object = GObjects->GetByIndex(i);
			if (!Object)
				continue;

			if (Object->GetName() == Name && Object->Outer()->GetName() == Outer)
				return static_cast<UEType*>(Object);
		}

		return nullptr;
	}
}