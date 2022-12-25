#pragma once

#include <string>

#define M_PI 3.14159265358979323846f

class IntersectionResultVec2;
class IntersectionResultVec3;
class ProjectionInfo;
class Vec3;

class Vec2
{
public:
	float x, y;
	Vec2() { x = y = 0.f; };
	Vec2(float x, float y) : x(x), y(y) {};

	Vec2					operator+(Vec2 other);
	Vec2					operator+(float val);
	Vec2					operator-(Vec2 other);
	Vec2					operator-(float val);
	Vec2					operator*(float val);
	Vec2					operator*(Vec2 other);
	Vec2					operator/(Vec2 other);
	Vec2					operator/(float val);
	bool					operator==(Vec2 other);

	bool					IsValid();
	float					Length();
	float					LengthSquared();
	float					Distance(Vec2 o);
	float					DistanceSquared(Vec2 o);
	Vec2					Vscale(Vec2 s);
	Vec2					Scale(float s);
	Vec2					Normalized();
	Vec2					Append(Vec2 pos1, Vec2 pos2, float dist);
	Vec2					Rotated(float angle);
	Vec2					Add(Vec2 o);
	Vec2					Sub(Vec2 o);
	Vec2					Mult(Vec2 o);
	Vec2					Div(Vec2 o);
	Vec2					Clone();
	float					Polar();
	float					CrossProduct(Vec2 other);
	float					AngleBetween(Vec2 o);
	IntersectionResultVec2	Intersection(Vec2 lineSegment1End, Vec2 lineSegment2Start, Vec2 lineSegment2End);
	ProjectionInfo			ProjectOn(Vec2 segmentStart, Vec2 segmentEnd);
	Vec2					Extend(Vec2& to, float distance);
	bool					IsVisibleOnScreen();
	Vec3					To3D(float height = 0);
	std::string				ToString();
};

class Vec3
{
public:
	float x, y, z;
	Vec3() { x = y = z = 0.f; };
	Vec3(float x, float y, float z = 0) : x(x), y(y), z(z) {};

	Vec2					ToScreen();

	Vec3					operator+(Vec3 v);
	Vec3					operator+(float mod);
	Vec3					operator-(Vec3 v);
	Vec3					operator-(float mod);
	Vec3					operator*(Vec3 v);
	Vec3					operator*(float mod);
	bool					operator!=(Vec3 v);
	bool					operator==(Vec3 v);

	bool					IsValid();
	Vec3					SwitchYZ();
	Vec3					Negate();
	float					DotProduct(Vec3 other);
	float					CrossProduct(Vec3 other);
	float					Polar();
	float					AngleBetween(Vec3 other);
	bool					Close(float a, float b, float eps);
	Vec3					Rotated(float angle);
	Vec3					Perpendicular();
	Vec3					Append(Vec3 pos1, Vec3 pos2, float dist);
	Vec3					Clone();
	float					Length();
	float					LengthSquared();
	float					Distance(Vec3 to);
	float					DistanceSquared(Vec3 to);
	Vec3					Extend(Vec3& to, float distance);
	Vec3					Scale(float s);
	Vec3					Rotate(Vec3 startPos, float theta);
	Vec3					RotateX(float angle);
	Vec3					RotateY(float angle);
	Vec3					RotateZ(float angle);
	Vec3					Normalized();
	Vec3					Add(Vec3 o);
	Vec3					Sub(Vec3 o);
	Vec3					Mult(Vec3 o);
	Vec3					Div(Vec3 o);
	Vec2					To2D();
	std::string				ToString();
};

class Vec4
{
public:
	float     x, y, z, w;
	Vec4() { x = y = z = w = 0.0f; };
	Vec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};

class ProjectionInfo
{
public:
	bool m_bIsOnSegment;
	Vec2 m_vLinePoint;
	Vec2 m_vSegmentPoint;

	ProjectionInfo(bool isOnSegement, Vec2 linePoint, Vec2 segmentPoint)
		:
		m_bIsOnSegment(isOnSegement),
		m_vLinePoint(linePoint),
		m_vSegmentPoint(segmentPoint)
	{};

};

class IntersectionResultVec2
{
public:
	bool m_bIntersects;
	Vec2 m_vIntersectionPoint;
	//bool m_bIsNull;
	int m_bIsNull;

	IntersectionResultVec2(bool b, Vec2 i) : m_bIntersects(b), m_vIntersectionPoint(i), m_bIsNull(0) {};
	IntersectionResultVec2(int val) {
		m_bIntersects = false;
		m_vIntersectionPoint = Vec2{ 0, 0 };
		m_bIsNull = val;
	};
};

class IntersectionResultVec3
{
public:
	bool m_bIntersects;
	Vec3 m_vIntersectionPoint;
};