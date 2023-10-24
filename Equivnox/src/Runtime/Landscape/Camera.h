#pragma once

#include <CoreMinimal.h>

namespace EQX {

	struct Camera
	{
		friend class Renderer;
	private:
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

		inline void SetPos(Vec4 pos) { this->pos = pos; }
		inline void SetPos(Vec3 pos) { this->pos = pos.ToVec4(); }

		// Make camera look at the designated position
		inline void LookAt(Vec3 pos) { this->lookAt = (this->pos + pos.Neg()).Neg(); }
		inline void LookAt(Vec4 pos) { this->LookAt(pos.ToVec3()); }

		inline void SetUp(Vec4 up) { this->upDir = up; }
		inline void SetUp(Vec3 up) { this->upDir = up.ToVec4(); }

		inline Vec3 GetPos() { return this->pos; }
	};

}

