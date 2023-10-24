#pragma once

// All Logging functions should be inlined as this header is included multiple times throughout the project

#include <iostream>
#include <string>

#include "Math/MathHeader.h"

namespace EQX {

#ifdef EQX_DEBUG
#define EQX_LOG(x) {cout << "| Variable: " << #x << " " << "| Type: " << typeid(x).name() << endl;\
	cout << "|_Value: "; Print(x);}

	inline void Print(std::string msg);
	inline void Print(int num);
	inline void Print(unsigned int num);
	inline void Print(size_t num);
	inline void Print(float num);
	inline void Print(double num);

	inline void Print(const Vector3&);
	inline void Print(const Vector4&);

	inline void Print(std::string msg)
	{
		cout << msg << endl;
	}

	inline void Print(int num)
	{
		cout << num << endl;
	}

	inline void Print(unsigned int num)
	{
		cout << num << endl;
	}

	inline void Print(size_t num)
	{
		cout << num << endl;
	}

	inline void Print(float num)
	{
		cout << num << endl;
	}

	inline void Print(double num)
	{
		cout << num << endl;
	}

	inline void Print(const Vector3& vec)
	{
		cout << vec.x << " " << vec.y << " " << vec.z << " " << endl;
	}

	inline void Print(const Vector4& vec)
	{
		cout << vec.x << " " << vec.y << " " << vec.z << " " << endl;
	}
#endif

}
