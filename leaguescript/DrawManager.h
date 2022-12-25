#pragma once

#include <d3dx9.h>
#include "Vector.h"

class DrawManager
{
private:
	IDirect3DDevice9* m_pDevice;

public:
	DrawManager(IDirect3DDevice9* device) : m_pDevice(device) {}

	void		ScreenRectangle(Vec2 p1, Vec2 p2, D3DCOLOR col, float rounding, float thickness, bool filled);
	void		DrawRectangle2Points(const Vec3& start, const Vec3& end, D3DCOLOR color, const int width, const float rounding = 0.f, const float thickness = 0.f);
	void		DrawRectangle4Points(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4, const float thickness, D3DCOLOR color);
	void		Line(Vec3 p1, Vec3 p2, float thickness, D3DCOLOR col);
	void		ScreenString(Vec2 pos, std::string text, D3DCOLOR& color, bool useSmallerFont = false);
	void		String(Vec3 pos, std::string text, D3DCOLOR& color, bool useSmallerFont = false);
	void		ScreenCircle(Vec2 center, const float radius, D3DCOLOR color, const bool filled, const int numPoints, const float thickness);
	void		Circle(Vec3 center, const float radius, D3DCOLOR color, bool filled, const int numPoints, const float thickness);
};