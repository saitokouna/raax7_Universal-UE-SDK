#pragma once

#define CREATE_GETTER_SETTER(Type, Member, Offset)          \
    Type Member()                                           \
    {                                                       \
        return *(Type*)((uintptr_t)this + Offset);          \
    }                                                       \
    void Member(Type value)                                 \
    {                                                       \
        *(Type*)((uintptr_t)this + Offset) = value;         \
    }