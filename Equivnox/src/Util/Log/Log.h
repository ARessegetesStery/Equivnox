#pragma once

#include <iostream>
#include <string>
#include "Math/MathHeader.h"

#include "Renderer/Geometry/Geometry.h"

namespace EQX {

#ifdef EQX_DEBUG
#define EQX_LOG(x) {cout << "| Variable: " << #x << " " << "| Type: " << typeid(x).name() << endl;\
	cout << "|_"; Print(x);}

	void Print(std::string msg);
	void Print(int num);
	void Print(unsigned int num);
	void Print(float num);
	void Print(double num);

	void Print(const Vector3&);
	void Print(const Vector4&);
	void Print(const Face&);
#endif

}
