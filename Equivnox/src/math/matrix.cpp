#include "matrix.h"

#include "eqxpch.h"

namespace EQX
{
	vec2::vec2() : x(0), y(0) {  }

	vec2::vec2(double x, double y) : x(x), y(y) {  }

	vec2::vec2(const vec2& v) : x(v.x), y(v.y) {  }

	vec2::vec2(vec2&& v) noexcept : x(v.x), y(v.y) {  }

	vec2 vec2::operator- (const vec2& p)
	{
		return vec2(this->x - p.x, this->y - p.y);
	}

	vec2 vec2::operator+ (const vec2& p)
	{
		return vec2(this->x + p.x, this->y + p.y);
	}

	vec2 vec2::operator= (const vec2& p)
	{
		return vec2(p);
	}

	vec3::vec3() : x(0), y(0), z(0) {  }

	vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {  }

	vec3::vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {  }

	vec3::vec3(vec3&& v) noexcept : x(v.x), y(v.y), z(v.z) {  }
}