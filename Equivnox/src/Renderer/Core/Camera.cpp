#include "eqxpch.h"

#include "Camera.h"

namespace EQX {

	Camera::Camera() : pos(Vector4::ZERO), lookAt(Vector4::ZERO), upDir(Vector4::ZERO), 
		width(0), height(0), nearClip(0.1), farClip(10), ZBufPrecision(9.9 / 255) {	}

	Camera::Camera(float nearClip, float farClip) : pos(Vector4::ZERO), lookAt(Vector4::ZERO), 
		upDir(Vector4::ZERO), width(0), height(0), nearClip(nearClip), farClip(farClip)
	{
		this->ZBufPrecision = abs(nearClip - farClip) / 255;
	}

	Camera::Camera(const Camera& c) : pos(c.pos), lookAt(c.lookAt), upDir(c.upDir),
		width(c.width), height(c.height), nearClip(c.nearClip), farClip(c.farClip), ZBufPrecision(c.ZBufPrecision)	{	}

	void Camera::fromFoV(float FoV, float aspect)
	{
		this->height = 2 * tan(FoV * EQXPI / 180.f) * nearClip;
		this->width = aspect * height;
	}
}
