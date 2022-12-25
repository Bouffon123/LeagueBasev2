#pragma once

#include "Imgui/imgui.h"

class DirectX
{
public:
	void Init();
	void StartFrame();
	void EndFrame();

	ImFont* editorFont;
	ImFont* ingameFont;
};

extern DirectX* g_dx;
