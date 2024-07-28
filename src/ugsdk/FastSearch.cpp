#include <private/Settings.hpp>
#include <ugsdk/FastSearch.hpp>
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/UnrealObjects.hpp>
#include <ugsdk/UnrealTypes.hpp>

namespace SDK
{
    bool FastSearch(std::vector<FSEntry>& SearchList)
    {
        // We require all of these functionalities.
        if (!Settings::SetupFMemory || !Settings::SetupGObjects || !Settings::SetupAppendString || !Settings::SetupFNameConstructor) {
            return false;
        }

        for (int i = 0; (i < GObjects->Num()) && SearchList.size(); i++) {
            UObject* Obj = GObjects->GetByIndex(i);
            if (!Obj)
                continue;

            for (auto It = SearchList.begin(); It != SearchList.end();) {
                bool DontIncrement = false;

                switch (It->Type) {
                case FS_UOBJECT: {
                    if (Obj->HasTypeFlag(It->Object.RequiredType) && Obj->Name() == It->Object.ObjectName) {
                        *It->Object.OutObject = Obj;

                        It = SearchList.erase(It);
                        DontIncrement = true;
                    }
                    break;
                }
                case FS_UPROPERTY: {
                    if (!Obj->HasTypeFlag(CASTCLASS_UStruct) || Obj->Name() != It->Property.ClassName)
                        break;

                    SDK::UStruct* ObjStruct = reinterpret_cast<SDK::UStruct*>(Obj);
                    PropertyInfo Info = ObjStruct->FindProperty(It->Property.PropertyName);
                    if (!Info.Found)
                        break;

                    if (It->Property.OutOffset)
                        *It->Property.OutOffset = Info.Offset;
                    if (It->Property.OutMask)
                        *It->Property.OutMask = Info.ByteMask;

                    It = SearchList.erase(It);
                    DontIncrement = true;
                    break;
                }
                case FS_UFUNCTION: {
                    if (!Obj->HasTypeFlag(CASTCLASS_UStruct) || Obj->Name() != It->Function.ClassName)
                        break;

                    SDK::UStruct* ObjStruct = reinterpret_cast<SDK::UStruct*>(Obj);
                    UFunction* Function = ObjStruct->FindFunction(It->Function.FunctionName);
                    if (!Function)
                        break;

                    *It->Function.OutFunction = Function;

                    It = SearchList.erase(It);
                    DontIncrement = true;
                    break;
                }

                default:
                    ++It;
                    break;
                }

                if (!DontIncrement)
                    ++It;
            }
        }

        return SearchList.size() == 0;
    }
}