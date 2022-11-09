#pragma once

namespace EQX {

	class vec2
	{
	private:
		double x;
		double y;
	public:
		vec2();
		vec2(double, double);
		vec2(const vec2&);
		vec2(vec2&&) noexcept;

		inline double getX() { return x; }
		inline double getY() { return y; }
			
		inline void setX(double x) { this->x = x; }
		inline void setY(double y) { this->y = y; }
	};
}