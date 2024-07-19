#include <libhat.hpp>
#include <ugsdk/FMemory.hpp>
#include <ugsdk/UnrealObjects.hpp>
#include <ugsdk/ObjectArray.hpp>
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

namespace OffsetFinder
{
	int32_t FindUStructChildrenOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ SDK::GObjects->FindObjectFast("PlayerController"), SDK::GObjects->FindObjectFastInOuter("WasInputKeyJustReleased", "PlayerController") });
		Infos.push_back({ SDK::GObjects->FindObjectFast("Controller"), SDK::GObjects->FindObjectFastInOuter("UnPossess", "Controller") });

		if (SDK::Memory::FindOffset(Infos) == OFFSET_NOT_FOUND)
		{
			Infos.clear();

			Infos.push_back({ SDK::GObjects->FindObjectFast("Vector"), SDK::GObjects->FindObjectFastInOuter("X", "Vector") });
			Infos.push_back({ SDK::GObjects->FindObjectFast("Vector4"), SDK::GObjects->FindObjectFastInOuter("X", "Vector4") });
			Infos.push_back({ SDK::GObjects->FindObjectFast("Vector2D"), SDK::GObjects->FindObjectFastInOuter("X", "Vector2D") });
			Infos.push_back({ SDK::GObjects->FindObjectFast("Guid"), SDK::GObjects->FindObjectFastInOuter("A","Guid") });

			return SDK::Memory::FindOffset(Infos);
		}

		SDK::Settings::UsesFProperty = true;

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUFieldNextOffset()
	{
		uint8_t* KismetSystemLibraryChild = reinterpret_cast<uint8_t*>(SDK::GObjects->FindObjectFast<SDK::UStruct>("KismetSystemLibrary")->Children());
		uint8_t* KismetStringLibraryChild = reinterpret_cast<uint8_t*>(SDK::GObjects->FindObjectFast<SDK::UStruct>("KismetStringLibrary")->Children());

		return SDK::Memory::GetValidPointerOffset(KismetSystemLibraryChild, KismetStringLibraryChild, SDK::Offsets::UObject::Outer + 0x08, 0x48);
	}
	int32_t FindUStructSuperOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ SDK::GObjects->FindObjectFast("Struct"), SDK::GObjects->FindObjectFast("Field") });
		Infos.push_back({ SDK::GObjects->FindObjectFast("Class"), SDK::GObjects->FindObjectFast("Struct") });

		// Thanks to the ue4 dev who decided UStruct should be spelled Ustruct
		if (Infos[0].first == nullptr)
			Infos[0].first = Infos[1].second = SDK::GObjects->FindObjectFast("struct");

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUClassCastFlagsOffset()
	{
		std::vector<std::pair<void*, SDK::EClassCastFlags>> Infos;

		Infos.push_back({ SDK::GObjects->FindObjectFast("Actor"), SDK::CASTCLASS_AActor });
		Infos.push_back({ SDK::GObjects->FindObjectFast("Class"), SDK::CASTCLASS_UField | SDK::CASTCLASS_UStruct | SDK::CASTCLASS_UClass });

		return SDK::Memory::FindOffset(Infos);
	}
	int32_t FindUClassDefaultObjectOffset()
	{
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ SDK::GObjects->FindObjectFast("Object"), SDK::GObjects->FindObjectFast("Default__Object") });
		Infos.push_back({ SDK::GObjects->FindObjectFast("Field"), SDK::GObjects->FindObjectFast("Default__Field") });

		return SDK::Memory::FindOffset(Infos);
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
			return true;
		}

		const auto& FixedGObjects = hat::find_pattern(FixedGObjectsSignature, ".text");
		if (FixedGObjects.has_result())
		{
			SDK::Settings::ChunkedGObjects = false;
			SDK::GObjects = std::make_unique<SDK::TUObjectArray>(SDK::Settings::ChunkedGObjects, (void*)SDK::Memory::CalculateRVA((uintptr_t)FixedGObjects.get(), 3));
			return true;
		}

		return false;
	}
	bool FindAppendString()
	{
		std::byte* Address = SDK::Memory::FindStringRef("ForwardShadingQuality_");
		std::byte* End = Address + 0x60;

		if (!Address)
			return false;

		const std::vector<std::pair<hat::fixed_signature<10>, int>> Signatures = {
			{ hat::compile_signature<"48 8D ? ? 48 8D ? ? E8 ?">(), 9 },
			{ hat::compile_signature<"48 8D ? ? ? 48 8D ? ? E8">(), 14 },
			{ hat::compile_signature<"48 8D ? ? 49 8B ? E8 ? ?">(), 12 },
			{ hat::compile_signature<"48 8D ? ? ? 49 8B ? E8 ?">(), 13 }
		};

		for (const auto& Sig : Signatures)
		{
			std::byte* Result = SDK::Memory::FindPatternInRange(Address, End, Sig.first);
			if (Result)
			{
				SDK::Offsets::FName::AppendString = SDK::Memory::CalculateRVA((uintptr_t)Result, Sig.second);
				return true;
			}
		}

		return false;
	}

	SDK::SDKStatus SetupMemberOffsets()
	{
		// The order of these is important as there is a dependency chain.

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

		// ...

		UClass::DefaultObject = FindUClassDefaultObjectOffset();
		if (UClass::DefaultObject == OFFSET_NOT_FOUND)
			return SDK::SDK_FAILED_UCLASS_DEFAULTOBJECT;

		return SDK::SDK_SUCCESS;
	}
}