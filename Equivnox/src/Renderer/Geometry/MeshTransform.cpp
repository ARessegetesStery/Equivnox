#include "eqxpch.h"

#include "MeshTransform.h"

namespace EQX {

	MeshTransform::MeshTransform()
	{
		this->displacement = Vec3::Zero;
		this->scaleCoeff = Vec3::One;
		this->scaleRef = Vec3::Zero;
		this->rotAxis = Vec3::UnitZ;
		this->rotAngle = 0.f;
	}

}
