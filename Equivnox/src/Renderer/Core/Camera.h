#pragma once

#include "eqxpch.h"

namespace EQX {

	struct Camera
	{
	public:
		Vector4 pos;
		Vector4 lookAt;
		Vector4 upDir;
		float width;
		float height;
		float nearClip;
		float farClip;

	public:
		Camera();
		Camera(float nearClip, float farClip);
		Camera(const Camera&);

		void fromFoV(float FoV, float aspect); 
	};

}

