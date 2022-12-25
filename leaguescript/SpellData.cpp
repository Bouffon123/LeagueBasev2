#include "SpellData.h"

#include "RiotString.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS SpellData
	MAKEGET_STR_C(name)
	MAKEGET_C(float, missileSpeed)
#undef FOCUS_CLASS
}
}