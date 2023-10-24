#pragma once

// All Logging functions should be inlined as this header is included multiple times throughout the project

#include <iostream>
#include <string>

#include "Math/MathHeader.h"

namespace EQX {

#ifdef EQX_DEBUG
#define EQX_LOG(x) {cout << "| Variable: " << #x << " " << "| Type: " << typeid(x).name() << endl;\
	cout << "|_Value: "; Print(x);}

	inline void Print(XString msg);
	inline void Print(XInt num);
	inline void Print(XUInt num);
	inline void Print(size_t num);
	inline void Print(XFloat num);
	inline void Print(double num);

	inline void Print(const Vector3&);
	inline void Print(const Vector4&);

	inline void Print(XString msg)
	{
		cout << msg << endl;
	}

	inline void Print(XInt num)
	{
		cout << num << endl;
	}

	inline void Print(XUInt num)
	{
		cout << num << endl;
	}

	inline void Print(size_t num)
	{
		cout << num << endl;
	}

	inline void Print(XFloat num)
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
