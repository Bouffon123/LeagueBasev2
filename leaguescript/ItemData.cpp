#include "ItemData.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS ItemData
	MAKEGET_C(int, itemId)
	MAKEGET_C(float, abilityHaste)
	MAKEGET_C(float, health)
	MAKEGET_C(float, movementSpeed)
	MAKEGET_C(float, armor)
	MAKEGET_C(float, magicResistance)
	MAKEGET_C(float, ad)
	MAKEGET_C(float, ap)
	MAKEGET_C(float, attackSpeed)
#undef FOCUS_CLASS
}
}