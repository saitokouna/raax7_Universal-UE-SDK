#include <UGSDK.hpp>
#include <private/Private.hpp>

namespace UGSDK
{
	bool Init()
	{
		return Private::SetupFMemory();
	}
}