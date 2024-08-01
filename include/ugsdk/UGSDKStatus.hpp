#pragma once

namespace SDK
{
    enum class Status
    {
        Success = 0,

        // Function offsets and VFT indexes.
        //
        FMemoryRealloc,
        GObjects,
        NarrowFNameConstructor,
        WideFNameConstructor,
        AppendString,
        ProcessEvent,
        ConsoleCommand,

        // Class member offsets.
        //
        UField_Next,
        UStruct_SuperStruct,
        UStruct_Children,
        UStruct_ChildProperties,
        UStruct_PropertiesSize,
        UStrct_MinAlignment,
        UClass_ClassCastFlags,
        UClass_ClassDefaultObject,
        UEnum_Names,
        UFunction_FunctionFlags,
        UFunction_NumParms,
        UFunction_ParmsSize,
        UFunction_ReturnValueOffset,
        UFunction_FuncOffset,
        UProperty_Offset,
        UProperty_ElementSize,
        UProperty_PropertyFlags,
        UBoolProperty_Base,

        // FastSearch passes.
        //
        FastSearchPass1,
        FastSearchPass2,
        FastSearchPass3,
    };
}