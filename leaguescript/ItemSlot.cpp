#include "ItemSlot.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS ItemSlot
	MAKEGET_C(ItemInfo*, itemInfo)
#undef FOCUS_CLASS
}
}