#include "MouseOverEffectData.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS MouseOverEffectData
	MAKEGET_C(D3DCOLOR, color)
	MAKEGET_C(int, size)
	MAKEGET_C(int, blur)
#undef FOCUS_CLASS
}
}