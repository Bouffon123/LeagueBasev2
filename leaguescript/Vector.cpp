#include "Vector.h"

#include <intrin.h>

#include "Addresses.h"
#include "fp_call.h"
#include "r3dRenderer.h"


__forceinline float Sqrt(float x)
{
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}


Vec2 Vec2::operator+(Vec2 other)
{
	return Vec2(this->x + other.x, this->y + other.y);
}

Vec2 Vec2::operator+(float val)
{
	return Vec2(this->x + val, this->y + val);
}

Vec2 Vec2::operator-(Vec2 other)
{
	return Vec2(this->x - other.x, this->y - other.y);
}

Vec2 Vec2::operator-(float val)
{
	return Vec2(this->x - val, this->y - val);
}

Vec2 Vec2::operator*(float val)
{
	return Vec2(this->x * val, this->y * val);
}

Vec2 Vec2::operator*(Vec2 other)
{
	return Vec2(this->x * other.x, this->y * other.y);
}

Vec2 Vec2::operator/(Vec2 other)
{
	return Vec2(this->x / other.x, this->y / other.y);
}

Vec2 Vec2::operator/(float val)
{
	return Vec2(this->x / val, this->y / val);
}

bool Vec2::operator==(Vec2 other)
{
	return this->x == other.x && this->y == other.y;
}

bool Vec2::IsValid()
{
	return x != 0 && y != 0;
}

float Vec2::Length() {
	return Sqrt(x * x + y * y);
}

float Vec2::LengthSquared() {
	return this->x * this->x + this->y * this->y;
}

float Vec2::Distance(Vec2 o) {
	return Sqrt(pow(x - o.x, 2.f) + pow(y - o.y, 2.f));
}

float Vec2::DistanceSquared(Vec2 to) {
	Vec2 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;

	return delta.LengthSquared();
}

Vec2 Vec2::Vscale(Vec2 s) {
	return Vec2(x * s.x, y * s.y);
}

Vec2 Vec2::Scale(float s) {
	return Vec2(x * s, y * s);
}

Vec2 Vec2::Normalized() {
	auto const length = Length();
	if (length != 0)
	{
		auto const inv = 1.0f / length;
		return { this->x * inv, this->y * inv };
	}

	return *this;
}

Vec2 Vec2::Add(Vec2 o) {
	return Vec2(x + o.x, y + o.y);
}

Vec2 Vec2::Sub(Vec2 o) {
	return Vec2(x - o.x, y - o.y);
}

Vec2 Vec2::Mult(Vec2 o) {
	return Vec2(x * o.x, y * o.y);
}

Vec2 Vec2::Div(Vec2 o) {
	return Vec2(x / o.x, y / o.y);
}

Vec2 Vec2::Clone() {
	return Vec2(x, y);
}

float Vec2::Polar()
{
	if (fabsf(x - 0.f) <= FLT_MIN)
	{
		return (y > 0.f) ? 90.f : (y < 0.f) ? 270.f : 0.f;
	}

	float theta = atan(y / x) * (180.f / M_PI);
	if (x < 0)
	{
		theta += 180;
	}

	if (theta < 0)
	{
		theta += 360;
	}

	return theta;
}

float Vec2::CrossProduct(Vec2 other)
{
	return other.y * this->x - other.x * this->y;
}

float Vec2::AngleBetween(Vec2 o)
{
	float theta = Polar() - o.Polar();
	if (theta < 0)
	{
		theta = theta + 360;
	}

	if (theta > 180)
	{
		theta = 360 - theta;
	}

	return theta;
}

IntersectionResultVec2 Vec2::Intersection(Vec2 lineSegment1End, Vec2 lineSegment2Start, Vec2 lineSegment2End)
{
	float deltaACy = y - lineSegment2Start.y;
	float deltaDCx = lineSegment2End.x - lineSegment2Start.x;
	float deltaACx = x - lineSegment2Start.x;
	float deltaDCy = lineSegment2End.y - lineSegment2Start.y;
	float deltaBAx = lineSegment1End.x - x;
	float deltaBAy = lineSegment1End.y - y;

	auto denominator = (deltaBAx * deltaDCy) - (deltaBAy * deltaDCx);
	auto numerator = (deltaACy * deltaDCx) - (deltaACx * deltaDCy);

	if (fabs(denominator) < FLT_EPSILON)
	{
		if (fabs(numerator) < FLT_EPSILON)
		{
			// collinear. Potentially infinite intersection points.
			// Check and return one of them.
			if (x >= lineSegment2Start.x && x <= lineSegment2End.x)
			{
				return IntersectionResultVec2{ true, *this };
			}

			if (lineSegment2Start.x >= x && lineSegment2Start.x <= lineSegment1End.x)
			{
				return IntersectionResultVec2{ true, lineSegment2Start };
			}

			return IntersectionResultVec2{ 1 };
		}

		// parallel
		return IntersectionResultVec2{ 2 };
	}

	auto r = numerator / denominator;
	if (r < 0 || r > 1)
	{
		return IntersectionResultVec2{ 3 };
	}

	auto s = ((deltaACy * deltaBAx) - (deltaACx * deltaBAy)) / denominator;
	if (s < 0 || s > 1)
	{
		return IntersectionResultVec2{ 4 };
	}

	return IntersectionResultVec2(
		true,
		Vec2(
			(float)(x + (r * deltaBAx)),
			(float)(y + (r * deltaBAy))));
}

ProjectionInfo Vec2::ProjectOn(Vec2 segmentStart, Vec2 segmentEnd)
{
	auto cx = x;
	auto cy = y;
	auto ax = segmentStart.x;
	auto ay = segmentStart.y;
	auto bx = segmentEnd.x;
	auto by = segmentEnd.y;
	auto rL = (((cx - ax) * (bx - ax)) + ((cy - ay) * (by - ay)))
		/ ((float)pow(bx - ax, 2) + (float)pow(by - ay, 2));
	auto pointLine = Vec2(ax + (rL * (bx - ax)), ay + (rL * (by - ay)));
	float rS;
	if (rL < 0)
	{
		rS = 0;
	}
	else if (rL > 1)
	{
		rS = 1;
	}
	else
	{
		rS = rL;
	}

	bool isOnSegment = rS == rL;
	auto pointSegment = isOnSegment ? pointLine : Vec2(ax + (rS * (bx - ax)), ay + (rS * (by - ay)));
	return ProjectionInfo(isOnSegment, pointSegment, pointLine);
}

Vec2 Vec2::Append(Vec2 pos1, Vec2 pos2, float dist)
{
	return pos2 + (pos2 - pos1).Normalized() * dist;
}

Vec2 Vec2::Rotated(float angle)
{
	auto const c = cos(angle);
	auto const s = sin(angle);

	return { static_cast<float>(x * c - y * s), static_cast<float>(y * c + x * s) };
}

Vec2 Vec2::Extend(Vec2& to, float distance)
{
	auto from = *this;
	auto result = from + (to.Sub(from)).Normalized().Mult(Vec2(distance, distance));
	return result;
}

bool Vec2::IsVisibleOnScreen()
{
	auto extraRadiusCheck = 25.f;
	auto screenSize = Script::Native::r3dRenderer::Instance()->rendererSize();

	return
		this->x + extraRadiusCheck >= 0
		&& this->x - extraRadiusCheck <= screenSize.x
		&& this->y + extraRadiusCheck >= 0
		&& this->y - extraRadiusCheck <= screenSize.y;
}

Vec3 Vec2::To3D(float height)
{
	return Vec3(x, height, y);
}

std::string Vec2::ToString()
{
	return "{ " + std::to_string(x) + " | " + std::to_string(y) + " }";
}

Vec2 Vec3::ToScreen()
{
	Vec3 in = { x,y,z };
	Vec2 out = Script::Native::r3dRenderer::WorldToScreen(in);

	return out;
}

Vec3 Vec3::operator+(Vec3 v)
{
	Vec3 result(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

Vec3 Vec3::operator+(float mod)
{
	Vec3 result(this->x + mod, this->y + mod, this->z + mod);
	return result;
}

Vec3 Vec3::operator-(Vec3 v)
{
	Vec3 result(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

Vec3 Vec3::operator-(float mod)
{
	Vec3 result(this->x - mod, this->y - mod, this->z - mod);
	return result;
}

Vec3 Vec3::operator*(Vec3 v)
{
	Vec3 result(this->x * v.x, this->y * v.y, this->z * v.z);
	return result;
}

Vec3 Vec3::operator*(float mod)
{
	Vec3 result(this->x * mod, this->y * mod, this->z * mod);
	return result;
}

bool Vec3::operator!=(Vec3 v)
{
	return v.x != x || v.y != y || v.z != z;
}

bool Vec3::operator==(Vec3 v)
{
	return v.x == x && v.y == y && v.z == z;
}

bool Vec3::IsValid()
{
	return this->x != 0/* && this->y != 0*/ && this->z != 0;
}

Vec3 Vec3::SwitchYZ()
{
	const auto temp = this->y;

	this->y = this->z;
	this->z = temp;
	return *this;
}

Vec3 Vec3::Negate()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;

	return *this;
}

float Vec3::DotProduct(Vec3 other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vec3::CrossProduct(Vec3 other)
{
	return other.y * this->x - other.x * this->y;
}

float Vec3::Polar()
{
	if (this->Close(x, 0.f, 0.f))
	{
		if (y > 0.f)
		{
			return 90.f;
		}
		return y < 0.f ? 270.f : 0.f;
	}

	auto theta = atan(y / x) * 180.f / M_PI;
	if (x < 0.f)
	{
		theta = theta + 180.f;
	}
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	return theta;
}

float Vec3::AngleBetween(Vec3 other)
{
	auto theta = Polar() - other.Polar();
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	if (theta > 180.f)
	{
		theta = 360.f - theta;
	}
	return theta;
}

bool Vec3::Close(float a, float b, float eps)
{
	if (fabs(eps) < FLT_EPSILON)
	{
		eps = static_cast<float>(1e-9);
	}
	return fabs(a - b) <= eps;
}

// Expects angle in radians!
Vec3 Vec3::Rotated(float angle)
{
	auto const c = cos(angle);
	auto const s = sin(angle);

	return { static_cast<float>(x * c - z * s), y, static_cast<float>(z * c + x * s) };
}

Vec3 Vec3::Perpendicular()
{
	return { -z,y,x };
}

Vec3 Vec3::Append(Vec3 pos1, Vec3 pos2, float dist)
{
	return pos2 + (pos2 - pos1).Normalized() * dist;
}

Vec3 Vec3::Clone()
{
	return Vec3(x, y, z);
}

float Vec3::Length()
{
	return Sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vec3::LengthSquared()
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vec3 Vec3::Normalized()
{
	auto const length = this->Length();
	if (length != 0)
	{
		auto const inv = 1.0f / length;
		return { this->x * inv, this->y * inv, this->z * inv };
	}

	return *this;
}

float Vec3::Distance(Vec3 to)
{
	Vec3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.z = z - to.z;

	return delta.Length();
}

float Vec3::DistanceSquared(Vec3 to)
{
	Vec3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.y = z - to.z;

	return delta.LengthSquared();
}

Vec3 Vec3::Extend(Vec3& to, float distance)
{
	auto from = *this;
	auto result = from + (to - from).Normalized() * distance;
	return result;
}

Vec3 Vec3::Scale(float s)
{
	return Vec3(x * s, y * s, z * s);
}

Vec3 Vec3::Rotate(Vec3 startPos, float theta)
{
	float dx = this->x - startPos.x;
	float dz = this->z - startPos.z;

	float px = dx * cos(theta) - dz * sin(theta);
	float pz = dx * sin(theta) + dz * cos(theta);
	return { px + startPos.x, this->y, pz + startPos.z };
}

Vec3 Vec3::RotateX(float angle) {
	return Vec3(
		x,
		y * cos(angle) - z * sin(angle),
		y * sin(angle) + z * cos(angle)
	);
}

Vec3 Vec3::RotateY(float angle) {
	return Vec3(
		x * cos(angle) + z * sin(angle),
		y,
		-x * sin(angle) + z * cos(angle)
	);
}

Vec3 Vec3::RotateZ(float angle) {
	return Vec3(
		x * cos(angle) - y * sin(angle),
		x * sin(angle) + y * cos(angle),
		z
	);
}

Vec3 Vec3::Add(Vec3 o) {
	return Vec3{ x + o.x, y + o.y, z + o.z };
}

Vec3 Vec3::Sub(Vec3 o) {
	return Vec3{ x - o.x, y - o.y, z - o.z };
}

Vec3 Vec3::Mult(Vec3 o) {
	return Vec3{ x * o.x, y * o.y, z * o.z };
}

Vec3 Vec3::Div(Vec3 o) {
	return Vec3{ x / o.x, y / o.y, z / o.z };
}

Vec2 Vec3::To2D() {
	return Vec2(x, z);
}

std::string Vec3::ToString()
{
	return "{ " + std::to_string(x) + " | " + std::to_string(y) + " | " + std::to_string(z) + " }";
}