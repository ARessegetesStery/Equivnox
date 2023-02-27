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

		/**
		 * Completes the parameters according to the given Field of View and Aspect Ratio
		 *
		 * @param FoV - field of view
		 * @param aspect - aspect ratio
		 */
		void fromFoV(const float FoV, const float aspect); 
	};

}

