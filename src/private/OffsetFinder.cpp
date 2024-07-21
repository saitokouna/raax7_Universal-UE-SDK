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

using namespace SDK::Offsets;

SDK::UObject* PlayerController = nullptr;
SDK::UObject* Controller = nullptr;

SDK::UObject* Vector = nullptr;
SDK::UObject* Vector4 = nullptr;
SDK::UObject* Vector2D = nullptr;
SDK::UObject* Guid = nullptr;

SDK::UObject* KismetSystemLibrary = nullptr;
SDK::UObject* KismetStringLibrary = nullptr;

SDK::UObject* Struct = nullptr;
SDK::UObject* Field = nullptr;
SDK::UObject* Class = nullptr;

SDK::UObject* Actor = nullptr;
SDK::UObject* Object = nullptr;

SDK::UObject* Default__Object = nullptr;
SDK::UObject* Default__Field = nullptr;

namespace OffsetFinder
{
	int32_t FindUStructChildrenOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ PlayerController, SDK::GObjects->FindObjectFastInOuter("WasInputKeyJustReleased", "PlayerController") });
		Infos.push_back({ Controller, SDK::GObjects->FindObjectFastInOuter("UnPossess", "Controller") });

		if (SDK::Memory::FindOffset(Infos) == OFFSET_NOT_FOUND)
		{
			Infos.clear();

			Infos.push_back({ Vector, SDK::GObjects->FindObjectFastInOuter("X", "Vector") });
			Infos.push_back({ Vector4, SDK::GObjects->FindObjectFastInOuter("X", "Vector4") });
			Infos.push_back({ Vector2D, SDK::GObjects->FindObjectFastInOuter("X", "Vector2D") });
			Infos.push_back({ Guid, SDK::GObjects->FindObjectFastInOuter("A","Guid") });

			return SDK::Memory::FindOffset(Infos);
		}

		SDK::Settings::UsesFProperty = true;

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUFieldNextOffset()
	{
		uint8_t* KismetSystemLibraryChild = reinterpret_cast<uint8_t*>(((SDK::UStruct*)(KismetSystemLibrary))->Children());
		uint8_t* KismetStringLibraryChild = reinterpret_cast<uint8_t*>(((SDK::UStruct*)(KismetStringLibrary))->Children());

		return SDK::Memory::GetValidPointerOffset(KismetSystemLibraryChild, KismetStringLibraryChild, SDK::Offsets::UObject::Outer + 0x08, 0x48);
	}
	int32_t FindUStructSuperOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ Struct, Field });
		Infos.push_back({ Class, Struct });

		// Thanks to the ue4 dev who decided UStruct should be spelled Ustruct
		if (Infos[0].first == nullptr)
			Infos[0].first = Infos[1].second = SDK::GObjects->FindObjectFast("struct");

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUClassCastFlagsOffset()
	{
		std::vector<std::pair<void*, SDK::EClassCastFlags>> Infos;

		Infos.push_back({ Actor, SDK::CASTCLASS_AActor });
		Infos.push_back({ Class, SDK::CASTCLASS_UField | SDK::CASTCLASS_UStruct | SDK::CASTCLASS_UClass });

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUClassDefaultObjectOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ Object, Default__Object });
		Infos.push_back({ Field, Default__Field });

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUPropertyOffsetOffset()
	{
		std::vector<std::pair<void*, int32_t>> Infos;

		SDK::UStruct* Color = SDK::GObjects->FindObjectFast<SDK::UStruct>("Color", SDK::CASTCLASS_UStruct);

		Infos.push_back({ Color->FindMember("B"), 0x00 });
		Infos.push_back({ Color->FindMember("G"), 0x01 });
		Infos.push_back({ Color->FindMember("R"), 0x02 });

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUBoolPropertyBaseOffset()
	{
		std::vector<std::pair<void*, uint8_t>> Infos;

		SDK::UClass* Engine = SDK::GObjects->FindClassFast("Engine");
		Infos.push_back({ Engine->FindMember("bIsOverridingSelectedColor"), 0xFF });
		Infos.push_back({ Engine->FindMember("bEnableOnScreenDebugMessagesDisplay"), 0b00000010 });
		Infos.push_back({ SDK::GObjects->FindClassFast("PlayerController")->FindMember("bAutoManageActiveCameraTarget"), 0xFF });

		return (SDK::Memory::FindOffset<1>(Infos, SDK::Offsets::UProperty::Offset) - 0x3);
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
		// The order of these is important as there is a dependency chain.
		// Do not change the order.

		std::vector<SDK::FSEntry> Search = {
			{ SDK::FSUObject("PlayerController", SDK::CASTCLASS_None, &PlayerController) },
			{ SDK::FSUObject("Controller", SDK::CASTCLASS_None, &Controller) },

			{ SDK::FSUObject("Vector", SDK::CASTCLASS_None, &Vector) },
			{ SDK::FSUObject("Vector4", SDK::CASTCLASS_None, &Vector4) },
			{ SDK::FSUObject("Vector2D", SDK::CASTCLASS_None, &Vector2D) },
			{ SDK::FSUObject("Guid", SDK::CASTCLASS_None, &Guid) },

			{ SDK::FSUObject("KismetSystemLibrary", SDK::CASTCLASS_None, &KismetSystemLibrary) },
			{ SDK::FSUObject("KismetStringLibrary", SDK::CASTCLASS_None, &KismetStringLibrary) },

			{ SDK::FSUObject("Struct", SDK::CASTCLASS_None, &Struct) },
			{ SDK::FSUObject("Field", SDK::CASTCLASS_None, &Field) },
			{ SDK::FSUObject("Class", SDK::CASTCLASS_None, &Class) },

			{ SDK::FSUObject("Actor", SDK::CASTCLASS_None, &Actor) },
			{ SDK::FSUObject("Object", SDK::CASTCLASS_None, &Object) },

			{ SDK::FSUObject("Default__Object", SDK::CASTCLASS_None, &Default__Object) },
			{ SDK::FSUObject("Default__Field", SDK::CASTCLASS_None, &Default__Field) },
		};

		if (!SDK::FastSearch(Search))
			return SDK::SDK_FAILED_FASTSEARCH;



		UStruct::Children = FindUStructChildrenOffset();
		if (UStruct::Children == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_UCLASS_CHILDREN;

		UField::Next = FindUFieldNextOffset();
		if (UField::Next == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_UFIELD_NEXT;

		UStruct::Super = FindUStructSuperOffset();
		if (UStruct::Super == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_USRTUCT_SUPER;

		UClass::CastFlags = FindUClassCastFlagsOffset();
		if (UClass::CastFlags == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_UCLASS_CASTFLAGS;

		if (SDK::Settings::UsesFProperty)
		{

		}
		else
		{
			UProperty::Offset = FindUPropertyOffsetOffset();
			if (UProperty::Offset == OFFSET_NOT_FOUND)
				return SDK::SDK_FAILED_UPROPERTY_OFFSET;

			UBoolProperty::Base = FindUBoolPropertyBaseOffset();
			if (UBoolProperty::Base == OFFSET_NOT_FOUND)
				return SDK::SDK_FAILED_UBOOLPROPERTY_BASE;
		}

		// ...

		UClass::DefaultObject = FindUClassDefaultObjectOffset();
		if (UClass::DefaultObject == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_UCLASS_DEFAULTOBJECT;

		SDK::Settings::SetupMemberOffsets = true;
		return SDK::SDK_SUCCESS;
	}
}