#pragma once

#include "Common.h"
#include "MouseOverEffectData.h"

namespace Script {
namespace Native {
	class SCRIPT_API RenderPipelineLOL final
	{
	public:
		MAKEGET(void*, screenBuffer)
		MAKEGET(MouseOverEffectData*, defaultMouseOverEffectData)
	};
}
}