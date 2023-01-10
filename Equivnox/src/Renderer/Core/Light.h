#pragma once

#include "eqxpch.h"

#include "Math/MathHeader.h"
#include "Renderer/ExternalTools/Image.h"

namespace EQX
{
	enum class LightType
	{
		Point, Direction
	};

	class Light
	{
	public:
		LightType lightType;
		Vec3 Position;
		Vec3 Direction;
		float intensity;
		Color lightColor;

	public:
		Light(Vector3 pos_or_dir, Color color, float intensity, LightType type);

		static const Vec3 _LightPlaceHolder;
	};

}
