#include "DrawManager.h"

#include "DirectX.h"
#include "Imgui/imgui.h"
#include "r3dRenderer.h"
#include "Imgui/imgui_internal.h"

void DrawManager::ScreenRectangle(Vec2 p1, Vec2 p2, D3DCOLOR col, float rounding, float thickness, bool filled)
{
	ImVec2 coord1{ p1.x, p1.y };
	ImVec2 coord2{ p2.x, p2.y };
	if (!filled)
		ImGui::GetWindowDrawList()->AddRect(coord1, coord2, col, rounding, 0, thickness);
	else
		ImGui::GetWindowDrawList()->AddRectFilled(coord1, coord2, col, rounding, 0);
}

void DrawManager::DrawRectangle2Points(const Vec3& start, const Vec3& end, D3DCOLOR color, const int width, const float rounding, const float thickness)
{
	auto dir = Vec3(end.x - start.x, 0, end.z - start.z).Normalized();

	auto leftDir = Vec3(dir.x, dir.y, dir.z).RotateY(90).Scale(width);
	auto rightDir = Vec3(dir.x, dir.y, dir.z).RotateY(-90).Scale(width);

	auto p1 = Vec3(start.x + leftDir.x, start.y + leftDir.y, start.z + leftDir.z);
	auto p2 = Vec3(end.x + leftDir.x, end.y + leftDir.y, end.z + leftDir.z);
	auto p3 = Vec3(end.x + rightDir.x, end.y + rightDir.y, end.z + rightDir.z);
	auto p4 = Vec3(start.x + rightDir.x, start.y + rightDir.y, start.z + rightDir.z);

	DrawRectangle4Points(p1, p2, p3, p4, thickness, color);
}

void DrawManager::DrawRectangle4Points(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4, const float thickness, D3DCOLOR color)
{
	Line(p1, p2, thickness, color);
	Line(p2, p3, thickness, color);
	Line(p3, p4, thickness, color);
	Line(p4, p1, thickness, color);
}

void DrawManager::Line(Vec3 p1, Vec3 p2, float thickness, D3DCOLOR col)
{
	int distance{ static_cast<int>(p1.Distance(p2)) };
	int segmentLength{ 25 };
	int rest = static_cast<int>(p1.Distance(p2)) % segmentLength;

	for (int i{}; i <= distance; i += segmentLength) {
		Vec2 start{ Script::Native::r3dRenderer::WorldToScreen(p1.Extend(p2, i)) };
		Vec2 end{ Script::Native::r3dRenderer::WorldToScreen(p1.Extend(p2, i + segmentLength > distance ? i + rest : i + segmentLength)) };
		Vec2 center{ (start + end) / 2 };

		if (center.IsVisibleOnScreen())
			ImGui::GetWindowDrawList()->AddLine(ImVec2{ start.x, start.y }, ImVec2{ end.x, end.y }, col, thickness);
	}
}

void DrawManager::ScreenCircle(Vec2 center, const float radius, D3DCOLOR color, const bool filled, const int numPoints, const float thickness)
{
	if (filled)
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(center.x, center.y), radius, color, numPoints);
	else
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(center.x, center.y), radius, color, numPoints, thickness);
}

void DrawManager::Circle(Vec3 center, const float radius, D3DCOLOR color, bool filled, const int numPoints, const float thickness)
{
	Vec3 world_pos(center.x, center.y, center.z);
	for (auto i = 0; i <= 99; i++) {
		auto angle = (float)i * M_PI * 1.98f / 98.0f;
		world_pos.x = center.x + ImCos(angle) * radius;
		world_pos.z = center.z + ImSin(angle) * radius;
		Vec2 pos = Script::Native::r3dRenderer::WorldToScreen(world_pos);
		ImGui::GetWindowDrawList()->PathLineTo(ImVec2(pos.x, pos.y));
	}
	if (!filled)
		ImGui::GetWindowDrawList()->PathStroke(color, 0, thickness);
	else
		ImGui::GetWindowDrawList()->PathFillConvex(color);
}

void DrawManager::ScreenString(Vec2 pos, std::string text, D3DCOLOR& color, bool useSmallerFont)
{
	if (useSmallerFont)
		ImGui::PushFont(g_dx->ingameFont);

	ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x, pos.y), color, text.c_str());

	if (useSmallerFont)
		ImGui::PopFont();
}

void DrawManager::String(Vec3 pos, std::string text, D3DCOLOR& color, bool useSmallerFont)
{
	if (useSmallerFont)
		ImGui::PushFont(g_dx->ingameFont);

	auto w2sPos = Script::Native::r3dRenderer::WorldToScreen(pos);
	ImGui::GetWindowDrawList()->AddText(ImVec2(w2sPos.x, w2sPos.y), color, text.c_str());

	if (useSmallerFont)
		ImGui::PopFont();
}

