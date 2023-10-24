#include "eqxpch.h"

#include "Camera.h"

namespace EQX {

	Camera::Camera() : pos(Vector4::Zero), lookAt(Vector4::Zero), upDir(Vector4::Zero), 
		width(0), height(0), nearClip(0.1f), farClip(10) {	}

	Camera::Camera(XFloat nearClip, XFloat farClip) : pos(Vector4::Zero), lookAt(Vector4::Zero), 
		upDir(Vector4::Zero), width(0), height(0), nearClip(nearClip), farClip(farClip) {	}

	Camera::Camera(const Camera& c) : pos(c.pos), lookAt(c.lookAt), upDir(c.upDir),
		width(c.width), height(c.height), nearClip(c.nearClip), farClip(c.farClip)	{	}

	void Camera::fromFoV(const XFloat FoV, const XFloat aspect)
	{
		this->height = 2 * tan(FoV * EQX_PI / 180.f) * nearClip;
		this->width = aspect * height;
	}
}
