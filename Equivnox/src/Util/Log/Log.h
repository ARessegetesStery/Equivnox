#pragma once

#include <iostream>
#include <string>
#include "Math/MathHeader.h"

#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Landscape/Landscape.h"

namespace EQX {

#ifdef EQX_DEBUG
#define EQX_LOG(x) {cout << "| Variable: " << #x << " " << "| Type: " << typeid(x).name() << endl;\
	cout << "|_Value: "; Print(x);}

	void Print(std::string msg);
	void Print(int num);
	void Print(unsigned int num);
	void Print(size_t num);
	void Print(float num);
	void Print(double num);

	void Print(const Vector3&);
	void Print(const Vector4&);
	void Print(const Face&);

	void Print(const SceneInfo&);
	void Print(const EntityInfo&);
#endif

}
