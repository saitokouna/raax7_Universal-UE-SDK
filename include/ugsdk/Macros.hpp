#pragma once
#include <type_traits>

#define OFFSET_NOT_FOUND -1

#define DECLARE_GETTER_SETTER(Type, Member)					\
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

#define ENUM_OPERATORS(EEnumClass)																																			\
	inline constexpr EEnumClass operator|(EEnumClass Left, EEnumClass Right)																								\
	{																																										\
		return (EEnumClass)((std::underlying_type<EEnumClass>::type)(Left) | (std::underlying_type<EEnumClass>::type)(Right));												\
	}																																										\
																																											\
	inline constexpr EEnumClass& operator|=(EEnumClass& Left, EEnumClass Right)																								\
	{																																										\
		return (EEnumClass&)((std::underlying_type<EEnumClass>::type&)(Left) |= (std::underlying_type<EEnumClass>::type)(Right));											\
	}																																										\
																																											\
	inline bool operator&(EEnumClass Left, EEnumClass Right)																												\
	{																																										\
		return (((std::underlying_type<EEnumClass>::type)(Left) & (std::underlying_type<EEnumClass>::type)(Right)) == (std::underlying_type<EEnumClass>::type)(Right));		\
	}