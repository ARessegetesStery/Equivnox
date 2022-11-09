#include "matrix.h"

namespace EQX
{
	vec2::vec2() : x(0), y(0) { this->x = 0; }

	vec2::vec2(double x, double y) : x(x), y(y) {  }

	vec2::vec2(const vec2& v) : x(v.x), y(v.y) {  }

	vec2::vec2(vec2&& v) noexcept : x(v.x), y(v.y) {  }
}