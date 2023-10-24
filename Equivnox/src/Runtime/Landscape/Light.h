#pragma once

#include <CoreMinimal.h>

#include "Runtime/ExternalTools/Image.h"

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
		Vec3 pos;
		Vec3 Direction;
		XFloat intensity;
		Color lightColor;

	public:
		Light(Vector3 pos_or_dir, Color color, XFloat intensity, LightType type);

		static const Vec3 _LightPlaceHolder;
	};

}
