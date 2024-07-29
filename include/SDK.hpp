#pragma once

#include <SDKStatus.hpp>
#include <ugsdk/FMemory.hpp>
#include <ugsdk/FastSearch.hpp>
#include <ugsdk/ObjectArray.hpp>
#include <ugsdk/Settings.hpp>
#include <ugsdk/UnrealContainers.hpp>
#include <ugsdk/UnrealEnums.hpp>
#include <ugsdk/UnrealObjects.hpp>

namespace SDK
{
    /**
    * @brief Initiates the core SDK. Should be called before any other interaction with the library.
    * @return An SDKStatus result. Should be compared with SDK_SUCCESS.
    */
    SDKStatus Init();
}