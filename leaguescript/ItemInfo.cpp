#include "ItemInfo.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS ItemInfo
	MAKEGET_C(ItemData*, itemData)
	MAKEGET_C(int, ammo)
	MAKEGET_C(float, price)
#undef FOCUS_CLASS
}
}