#pragma once
#include "Vector.h"
#include "Common.h"

#include <d3dx9.h>

namespace Script {
namespace Native {

	class SCRIPT_API r3dRenderer final
	{
	public:
		MAKEGET(D3DMATRIX, viewMatrix)
		MAKEGET(D3DMATRIX, projectionMatrix)
		MAKEGET(Vec2, rendererSize)

		static r3dRenderer* Instance();

		static bool ApplyHooks();

		static Vec2 WorldToScreen(Vec3 in);
	};
}
}
