#include "RenderPipelineLOL.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS RenderPipelineLOL
	MAKEGET_C(void*, screenBuffer)
	MAKEGET_C(MouseOverEffectData*, defaultMouseOverEffectData)
#undef FOCUS_CLASS
}
}