#pragma once

#include "eqxpch.h"

namespace EQX {

	class vec2
	{
	public:
		double x;
		double y;
	public:
		vec2();
		vec2(double, double);
		vec2(const vec2&);
		vec2(vec2&&) noexcept;

		vec2 operator- (const vec2& p);
		vec2 operator+ (const vec2& p);
		vec2 operator= (const vec2& p);
	};

	class vec3
	{
	public:
		double x;
		double y;
		double z;

		vec3();
		vec3(double, double, double);
		vec3(const vec3&);
		vec3(vec3&&) noexcept;
	};
}