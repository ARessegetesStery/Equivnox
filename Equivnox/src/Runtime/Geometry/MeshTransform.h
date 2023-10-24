#pragma once

#include <CoreMinimal.h>

namespace EQX {

	/**
	 * The sequence of applying the changes is
	 * Translation -> Scaling -> Rotation
	 */
	class MeshTransform
	{
	public:
		Vec3 displacement;
		Vec3 scaleCoeff;
		Vec3 scaleRef;
		XFloat rotAngle;
		Vec3 rotAxis;

	public:
		MeshTransform();
		MeshTransform(Vec3 d, Vec3 sc, Vec3 sr, XFloat angle, Vec3 axis);
	};

}
