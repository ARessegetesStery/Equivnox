#include "eqxpch.h"

#include "MeshTransform.h"

namespace EQX {

	MeshTransform::MeshTransform()
	{
		this->displacement = Vec3::Zero;
		this->scaleCoeff = Vec3::One;
		this->scaleRef = Vec3::Zero;
		this->rotAngle = 0.f;
		this->rotAxis = Vec3::UnitZ;
	}

	MeshTransform::MeshTransform(Vec3 d, Vec3 sc, Vec3 sr, float angle, Vec3 axis) : 
		displacement(d), scaleCoeff(sc), scaleRef(sr), rotAngle(angle), rotAxis(axis) {	}

}
