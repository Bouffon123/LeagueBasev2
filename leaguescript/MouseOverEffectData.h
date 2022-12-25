#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API MouseOverEffectData final
	{
	public:
		MAKEGET(D3DCOLOR, color)
		MAKEGET(int, size)
		MAKEGET(int, blur)
	};
}
}