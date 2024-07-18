#pragma once

#define DECLARE_GETTER_SETTER(Type, Member)         \
    Type Member();                                          \
    void Member(Type value);

#define DEFINE_GETTER_SETTER(Class, Type, Member, Offset)   \
    Type Class::Member()                                    \
    {                                                       \
        return *(Type*)((uintptr_t)this + Offset);          \
    }                                                       \
    void Class::Member(Type value)                          \
    {                                                       \
        *(Type*)((uintptr_t)this + Offset) = value;         \
    }