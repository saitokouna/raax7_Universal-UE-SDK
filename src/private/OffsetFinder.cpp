#include <libhat.hpp>
#include <ugsdk/FMemory.hpp>
#include <ugsdk/UnrealObjects.hpp>
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/FastSearch.hpp>
#include <private/Offsets.hpp>
#include <private/Settings.hpp>
#include <private/Memory.hpp>
#include <SDKStatus.hpp>

// Most of the offset finding is copied 1:1 from https://github.com/Encryqed/Dumper-7, so credits to them.
//
// Most people would consider this skidding, but I would think it's what open source is for.
// Someone has found the most optimal way to find something, why should I limit my library
// just to avoid copying them?

#define GET_OFFSET(Function, Variable, ErrorCode) \
    Variable = Function();                        \
    if (Variable == OFFSET_NOT_FOUND)             \
        return ErrorCode;

using namespace SDK::Offsets;

namespace OffsetFinder
{
	SDK::UObject* PlayerController = nullptr;
	SDK::UObject* Controller = nullptr;

	SDK::UObject* Vector = nullptr;
	SDK::UObject* Vector4 = nullptr;
	SDK::UObject* Vector2D = nullptr;
	SDK::UStruct* Guid = nullptr;

	SDK::UObject* KismetSystemLibrary = nullptr;
	SDK::UObject* KismetStringLibrary = nullptr;

	SDK::UObject* Struct = nullptr;
	SDK::UObject* Field = nullptr;
	SDK::UObject* Class = nullptr;

	SDK::UObject* Actor = nullptr;
	SDK::UObject* Object = nullptr;

	SDK::UObject* Default__Object = nullptr;
	SDK::UObject* Default__Field = nullptr;

	SDK::UStruct* Color = nullptr;

	SDK::UClass* Engine = nullptr;

	SDK::UFunction* SwitchLevel = nullptr;
	SDK::UFunction* SetViewTargetWithBlend = nullptr;
	SDK::UFunction* SetHapticsByValue = nullptr;
	SDK::UFunction* SphereTraceSingleForObjects = nullptr;

	SDK::UFunction* WasRecentlyRendered = nullptr;
	SDK::UFunction* CrossProduct2D = nullptr;
	SDK::UFunction* GetSpectatorPawn = nullptr;
}

namespace OffsetFinder
{
	int32_t Find_UField_Next()
	{
		uint8_t* KismetSystemLibraryChild = reinterpret_cast<uint8_t*>(((SDK::UStruct*)(KismetSystemLibrary))->Children());
		uint8_t* KismetStringLibraryChild = reinterpret_cast<uint8_t*>(((SDK::UStruct*)(KismetStringLibrary))->Children());

		return SDK::Memory::GetValidPointerOffset(KismetSystemLibraryChild, KismetStringLibraryChild, SDK::Offsets::UObject::Outer + 0x8, 0x48);
	}

	int32_t Find_UStruct_Children()
	{
		std::vector<std::pair<void*, void*>> ValuePair = {
			{ PlayerController, SDK::GObjects->FindObjectFastInOuter("WasInputKeyJustReleased", "PlayerController") },
			{ Controller, SDK::GObjects->FindObjectFastInOuter("UnPossess", "Controller") }
		};

		if (SDK::Memory::FindOffset(ValuePair) == OFFSET_NOT_FOUND)
		{
			ValuePair.clear();
			ValuePair = {
				{ Vector, SDK::GObjects->FindObjectFastInOuter("X", "Vector") },
				{ Vector4, SDK::GObjects->FindObjectFastInOuter("X", "Vector4") },
				{ Vector2D, SDK::GObjects->FindObjectFastInOuter("X", "Vector2D") },
				{ Guid, SDK::GObjects->FindObjectFastInOuter("A","Guid") }
			};

			return SDK::Memory::FindOffset(ValuePair);
		}

		SDK::Settings::UsesFProperty = true;
		return SDK::Memory::FindOffset(ValuePair);
	}
	int32_t Find_UStruct_Super()
	{
		std::vector<std::pair<void*, void*>> ValuePair = {
			{ Struct, Field },
			{ Class, Struct }
		};

		// In some UE4 versions, this is called Struct is not capitalized.
		if (ValuePair[0].first == nullptr)
			ValuePair[0].first = ValuePair[1].second = SDK::GObjects->FindObjectFast("struct");

		return SDK::Memory::FindOffset(ValuePair);
	}

	int32_t Find_UClass_CastFlags()
	{
		std::vector<std::pair<void*, SDK::EClassCastFlags>> ValuePair = {
			{ Actor, SDK::CASTCLASS_AActor },
			{ Class, SDK::CASTCLASS_UField | SDK::CASTCLASS_UStruct | SDK::CASTCLASS_UClass }
		};

		return SDK::Memory::FindOffset(ValuePair);
	}
	int32_t Find_UClass_DefaultObject()
	{
		std::vector<std::pair<void*, void*>> ValuePair = {
			{ Object, Default__Object },
			{ Field, Default__Field }
		};

		return SDK::Memory::FindOffset(ValuePair);
	}

	int32_t Find_UProperty_Offset()
	{
		std::vector<std::pair<void*, int32_t>> ValuePair = {
			{ Color->FindMember("B"), 0x00 },
			{ Color->FindMember("G"), 0x01 },
			{ Color->FindMember("R"), 0x02 }
		};

		return SDK::Memory::FindOffset(ValuePair);
	}
	int32_t Find_UProperty_ElementSize()
	{
		std::vector<std::pair<void*, int32_t>> ValuePair = {
			{ Guid->FindMember("A"), 0x04 },
			{ Guid->FindMember("B"), 0x04 },
			{ Guid->FindMember("C"), 0x04 }
		};

		return SDK::Memory::FindOffset(ValuePair);
	}
	int32_t Find_UProperty_PropertyFlags()
	{
		constexpr SDK::EPropertyFlags GuidMemberFlags = SDK::CPF_Edit | SDK::CPF_ZeroConstructor | SDK::CPF_SaveGame | SDK::CPF_IsPlainOldData | SDK::CPF_NoDestructor | SDK::CPF_HasGetValueTypeHash;
		constexpr SDK::EPropertyFlags ColorMemberFlags = SDK::CPF_Edit | SDK::CPF_BlueprintVisible | SDK::CPF_ZeroConstructor | SDK::CPF_SaveGame | SDK::CPF_IsPlainOldData | SDK::CPF_NoDestructor | SDK::CPF_HasGetValueTypeHash;

		std::vector<std::pair<void*, SDK::EPropertyFlags>> ValuePair = {
			{ Guid->FindMember("A"), GuidMemberFlags },
			{ Color->FindMember("R"), ColorMemberFlags }
		};

		int FlagsOffset = SDK::Memory::FindOffset(ValuePair);

		// Same flags without AccessSpecifier.
		if (FlagsOffset == OFFSET_NOT_FOUND)
		{
			ValuePair[0].second |= SDK::CPF_NativeAccessSpecifierPublic;
			ValuePair[1].second |= SDK::CPF_NativeAccessSpecifierPublic;

			FlagsOffset = SDK::Memory::FindOffset(ValuePair);
		}

		return FlagsOffset;
	}

	int32_t Find_UBoolProperty_Base()
	{
		std::vector<std::pair<void*, uint8_t>> ValuePair = {
			{ Engine->FindMember("bIsOverridingSelectedColor"), 0xFF },
			{ Engine->FindMember("bEnableOnScreenDebugMessagesDisplay"), 0b00000010 },
			{ SDK::GObjects->FindClassFast("PlayerController")->FindMember("bAutoManageActiveCameraTarget"), 0xFF }
		};

		return SDK::Memory::FindOffset<1>(ValuePair, SDK::Offsets::UProperty::Offset) - 0x3;
	}

	int32_t Find_UFunction_NumParms()
	{
		std::vector<std::pair<void*, uint8_t>> ValuePair = {
			{ SwitchLevel, 0x1 },
			{ SetViewTargetWithBlend, 0x5 },
			{ SetHapticsByValue, 0x3 },
			{ SphereTraceSingleForObjects, 0xE }
		};

		return SDK::Memory::FindOffset<1>(ValuePair);
	}
	int32_t Find_UFunction_ParmsSize()
	{
		std::vector<std::pair<void*, uint16_t>> ValuePair = {
			{ SwitchLevel, 0x10 },
			{ SetViewTargetWithBlend, 0x15 },
			{ SetHapticsByValue, 0x9 },
			{ SphereTraceSingleForObjects, 0x109 }
		};

		int ParamSizeOffset = SDK::Memory::FindOffset<1>(ValuePair);

		// This is from Dumper-7. Not sure why we do it, I don't think Dumper-7 knows either but I'll just leave it.
		if (SDK::Settings::UsesFProperty)
		{
			return ParamSizeOffset + 0x1;
		}

		return ParamSizeOffset;
	}
	int32_t Find_UFunction_ReturnValueOffset()
	{
		std::vector<std::pair<void*, uint16_t>> ValuePair = {
			{ WasRecentlyRendered, 0x4 },
			{ CrossProduct2D, 0x10 },
			{ GetSpectatorPawn, 0x0 },
			{ SphereTraceSingleForObjects, 0x108 }
		};

		return SDK::Memory::FindOffset<1>(ValuePair);
	}
}

namespace OffsetFinder
{
	bool FindFMemoryRealloc()
	{
		// This signature has been very reliable from what I have tested.
		// Since Realloc can be used as Alloc and Free aswell, it is easier to only get Realloc.
		/*
		48 89 5C 24		mov     [rsp+arg_0], rbx
		08
		48 89 74 24		mov     [rsp+arg_8], rsi
		10
		57				push    rdi
		48 83 EC 20		sub     rsp, 20h
		48 8B F1		mov     rsi, rcx
		41 8B D8		mov     ebx, r8d
		48 8B 0D 64		mov     rcx, cs:qword_6024FE0
		5A 10 04
		48 8B FA		mov     rdi, rdx
		48 85 C9		test    rcx, rcx
		*/
		constexpr hat::fixed_signature FMemorySignature = hat::compile_signature<"48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F1 41 8B D8 48 8B 0D ? ? ? ? 48 8B FA 48 85 C9">();

		const auto& Result = hat::find_pattern(FMemorySignature, ".text");
		SDK::Offsets::FMemory::Realloc = reinterpret_cast<uintptr_t>(Result.get());

		if (Result.has_result())
			SDK::Settings::SetupFMemory = true;

		return Result.has_result();
	}
	bool FindGObjects()
	{
		SDK::Settings::ChunkedGObjects = true;

		constexpr hat::fixed_signature ChunkedGObjectsSignature = hat::compile_signature<"48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1">();
		constexpr hat::fixed_signature FixedGObjectsSignature = hat::compile_signature<"48 8B 05 ? ? ? ? 48 8D 14 C8 EB 02">();

		const auto& ChunkedGObjects = hat::find_pattern(ChunkedGObjectsSignature, ".text");
		if (ChunkedGObjects.has_result())
		{
			SDK::Settings::ChunkedGObjects = true;
			SDK::GObjects = std::make_unique<SDK::TUObjectArray>(SDK::Settings::ChunkedGObjects, (void*)SDK::Memory::CalculateRVA((uintptr_t)ChunkedGObjects.get(), 3));

			SDK::Settings::SetupGObjects = true;
			return true;
		}

		const auto& FixedGObjects = hat::find_pattern(FixedGObjectsSignature, ".text");
		if (FixedGObjects.has_result())
		{
			SDK::Settings::ChunkedGObjects = false;
			SDK::GObjects = std::make_unique<SDK::TUObjectArray>(SDK::Settings::ChunkedGObjects, (void*)SDK::Memory::CalculateRVA((uintptr_t)FixedGObjects.get(), 3));

			SDK::Settings::SetupGObjects = true;
			return true;
		}

		return false;
	}
	bool FindFNameConstructor()
	{
		const auto& PatternResult = hat::find_pattern(hat::compile_signature<"41 ? 01 ? ? ? 48 8D 15 ? ? ? ? 48 8D 4D ? E8">(), ".text");
		if (!PatternResult.has_result())
			return false;

		SDK::Offsets::FName::Constructor = SDK::Memory::CalculateRVA((uintptr_t)PatternResult.get(), 18);

		SDK::Settings::SetupFNameConstructor = true;
		return true;
	}
	bool FindAppendString()
	{
		/*
		4C 8D 05 65 17 79 02	lea     r8, aForwardshading ; "ForwardShadingQuality_"
		41 B9 17 00 00 00		mov     r9d, 17h
		B8 3F 00 00 00			mov     eax, 3Fh ; '?'
		41 8B D1				mov     edx, r9d
		66 89 44 24 20			mov     word ptr [rsp+0A0h+var_80], ax
		E8 AD AF EE FD			call    sub_36C900
		48 8D 55 17				lea     rdx, [rbp+57h+var_40]
		48 8D 4D 6F				lea     rcx, [rbp+57h+arg_8]
		E8 40 DE B7 FF			call    AppendString
		*/

		std::byte* Start = SDK::Memory::FindStringRef("ForwardShadingQuality_");
		std::byte* End = Start + 0x60;

		if (!Start)
			return false;

		const std::vector<std::pair<hat::fixed_signature<10>, int>> Signatures = {
			{ hat::compile_signature<"48 8D ? ? 48 8D ? ? E8 ?">(), 9 },
			{ hat::compile_signature<"48 8D ? ? ? 48 8D ? ? E8">(), 14 },
			{ hat::compile_signature<"48 8D ? ? 49 8B ? E8 ? ?">(), 12 },
			{ hat::compile_signature<"48 8D ? ? ? 49 8B ? E8 ?">(), 13 }
		};

		for (const auto& Sig : Signatures)
		{
			std::byte* Result = SDK::Memory::FindPatternInRange(Start, End, Sig.first);
			if (Result)
			{
				SDK::Offsets::FName::AppendString = SDK::Memory::CalculateRVA((uintptr_t)Result, Sig.second);

				SDK::Settings::SetupAppendString = true;
				return true;
			}
		}

		return false;
	}

	SDK::SDKStatus SetupMemberOffsets()
	{
		std::vector<SDK::FSEntry> Search = {
			{ SDK::FSUObject("PlayerController", &PlayerController) },
			{ SDK::FSUObject("Controller", &Controller) },

			{ SDK::FSUObject("Vector", &Vector) },
			{ SDK::FSUObject("Vector4", &Vector4) },
			{ SDK::FSUObject("Vector2D", &Vector2D) },
			{ SDK::FSUObject("Guid", /*SDK::CASTCLASS_UStruct,*/ &Guid) },

			{ SDK::FSUObject("KismetSystemLibrary", &KismetSystemLibrary) },
			{ SDK::FSUObject("KismetStringLibrary", &KismetStringLibrary) },

			{ SDK::FSUObject("Struct", &Struct) },
			{ SDK::FSUObject("Field", &Field) },
			{ SDK::FSUObject("Class", &Class) },

			{ SDK::FSUObject("Actor", &Actor) },
			{ SDK::FSUObject("Object", &Object) },

			{ SDK::FSUObject("Default__Object", &Default__Object) },
			{ SDK::FSUObject("Default__Field", &Default__Field) },

			{ SDK::FSUObject("Color", /*SDK::CASTCLASS_UStruct,*/ &Color) },
			{ SDK::FSUObject("Engine", /*SDK::CASTCLASS_UClass,*/ &Engine) }
		};
		if (!SDK::FastSearch(Search))
			return SDK::SDK_FAILED_FASTSEARCH_PASS1;

		// The order of the functions below is very important as there is a dependency chain.
		// Do not change the order.

		GET_OFFSET(Find_UStruct_Children, UStruct::Children, SDK::SDK_FAILED_UCLASS_CHILDREN);
		GET_OFFSET(Find_UField_Next, UField::Next, SDK::SDK_FAILED_UFIELD_NEXT);
		GET_OFFSET(Find_UStruct_Super, UStruct::Super, SDK::SDK_FAILED_USRTUCT_SUPER);
		GET_OFFSET(Find_UClass_CastFlags, UClass::CastFlags, SDK::SDK_FAILED_UCLASS_CASTFLAGS);

		// In order to do the second pass, we required UClass::CastFlags.
		
		Search.clear();
		Search = {
			{ SDK::FSUFunction("PlayerController", "SwitchLevel", &SwitchLevel) },
			{ SDK::FSUFunction("PlayerController", "SetViewTargetWithBlend", &SetViewTargetWithBlend) },
			{ SDK::FSUFunction("PlayerController", "SetHapticsByValue", &SetHapticsByValue) },
			{ SDK::FSUFunction("KismetSystemLibrary", "SphereTraceSingleForObjects", &SphereTraceSingleForObjects) },
			{ SDK::FSUFunction("Actor", "WasRecentlyRendered", &WasRecentlyRendered) },
			{ SDK::FSUFunction("KismetMathLibrary", "CrossProduct2D", &CrossProduct2D) },
			{ SDK::FSUFunction("PlayerController", "GetSpectatorPawn", &GetSpectatorPawn) },
		};
		if (!SDK::FastSearch(Search))
			return SDK::SDK_FAILED_FASTSEARCH_PASS2;

		GET_OFFSET(Find_UFunction_NumParms, UFunction::NumParms, SDK::SDK_FAILED_UFUNCTION_NUMPARMS);
		GET_OFFSET(Find_UFunction_ParmsSize, UFunction::ParmsSize, SDK::SDK_FAILED_UFUNCTION_PARMSSIZE);
		GET_OFFSET(Find_UFunction_ReturnValueOffset, UFunction::ReturnValueOffset, SDK::SDK_FAILED_UFUNCTION_RETURNVALUEOFFSET);

		if (SDK::Settings::UsesFProperty)
		{
			throw std::runtime_error("FProperties are not supported yet!");
		}
		else
		{
			GET_OFFSET(Find_UProperty_Offset, UProperty::Offset, SDK::SDK_FAILED_UPROPERTY_OFFSET);
			GET_OFFSET(Find_UProperty_ElementSize, UProperty::ElementSize, SDK::SDK_FAILED_UPROPERTY_ELEMENTSIZE);
			GET_OFFSET(Find_UProperty_PropertyFlags, UProperty::PropertyFlags, SDK::SDK_FAILED_UPROPERTY_PROPERTYFLAGS);
			GET_OFFSET(Find_UBoolProperty_Base, UBoolProperty::Base, SDK::SDK_FAILED_UBOOLPROPERTY_BASE);
		}

		GET_OFFSET(Find_UClass_DefaultObject, UClass::DefaultObject, SDK::SDK_FAILED_UCLASS_DEFAULTOBJECT);

		SDK::Settings::SetupMemberOffsets = true;
		return SDK::SDK_SUCCESS;
	}
}