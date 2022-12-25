#pragma once

#include "Common.h"
#include "ItemData.h"

namespace Script {
namespace Native {
	class SCRIPT_API ItemInfo final
	{
	public:
		MAKEGET(ItemData*, itemData)
		MAKEGET(int, ammo)
		MAKEGET(float, price)
	};
}
}