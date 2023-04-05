#pragma once

#include "eqxpch.h"

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
		float rotAngle;
		Vec3 rotAxis;

	public:
		MeshTransform();
	};

}
