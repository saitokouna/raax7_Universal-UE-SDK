#pragma once

#include <ugsdk/FMemory.hpp>
#include <ugsdk/FastSearch.hpp>
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/PublicOffsets.hpp>
#include <ugsdk/Settings.hpp>
#include <ugsdk/UGSDKStatus.hpp>
#include <ugsdk/UnrealContainers.hpp>
#include <ugsdk/UnrealEnums.hpp>
#include <ugsdk/UnrealObjects.hpp>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

namespace SDK
{
    /**
     * @brief Initiates the core SDK. Should be called before any other interaction with the library.
     * @return SDK::Status result. Should be compared with SDK::Status::Success.
     */
    Status Init();
}