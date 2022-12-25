#pragma once

#include "Common.h"
#include "ItemInfo.h"

namespace Script {
namespace Native {
	class SCRIPT_API ItemSlot final
	{
	public:
		MAKEGET(ItemInfo*, itemInfo)
	};
}
}