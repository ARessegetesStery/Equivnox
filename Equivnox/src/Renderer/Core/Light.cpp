#include "Light.h"

namespace EQX
{
	const Vec3 Light::_LightPlaceHolder = Vec3::ZERO;
	Light::Light(Vector3 pos_or_dir, Color color, float intensity, LightType type)
	{
		this->lightType = type;
		this->lightColor = color;
		this->intensity = intensity;
		if (type == LightType::Point)
		{
			this->Direction = _LightPlaceHolder;
			this->Position = pos_or_dir;
		}
		else if (type == LightType::Direction)
		{
			this->Direction = pos_or_dir;
			this->Position = _LightPlaceHolder;
		}
	}
}
