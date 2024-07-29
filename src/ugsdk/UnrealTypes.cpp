#include <ugsdk/UnrealTypes.hpp>

namespace SDK
{
    bool FField::HasTypeFlag(EClassCastFlags TypeFlag) const
    {
        if (ClassPrivate == nullptr)
            return false;

        return TypeFlag != CASTCLASS_None ? ClassPrivate->CastFlags & TypeFlag : false;
    }

    bool FProperty::HasPropertyFlag(EPropertyFlags PropertyFlag) const
    {
        return PropertyFlags != CASTCLASS_None ? PropertyFlags & PropertyFlag : false;
    }

    bool FBoolProperty::IsNativeBool()
    {
        return GetFieldMask() == 0xFF;
    }
    uint8_t FBoolProperty::GetFieldMask()
    {
        return FieldMask;
    }
    uint8_t FBoolProperty::GetBitIndex()
    {
        uint8_t FieldMask = GetFieldMask();

        if (FieldMask != 0xFF) {
            if (FieldMask == 0x01) {
                return 0;
            }
            if (FieldMask == 0x02) {
                return 1;
            }
            if (FieldMask == 0x04) {
                return 2;
            }
            if (FieldMask == 0x08) {
                return 3;
            }
            if (FieldMask == 0x10) {
                return 4;
            }
            if (FieldMask == 0x20) {
                return 5;
            }
            if (FieldMask == 0x40) {
                return 6;
            }
            if (FieldMask == 0x80) {
                return 7;
            }
        }

        return 0xFF;
    }
}