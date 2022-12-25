#include "Cursor.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS Cursor
	MAKEGET_C(Vec3, position)
	MAKEGET_C(Vec3, lastClickPosition)
	MAKEGET_C(unsigned, hoveredUnitId)
#undef FOCUS_CLASS
}
}