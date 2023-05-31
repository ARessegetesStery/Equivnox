#include "eqxpch.h"

#include "Log.h"

namespace EQX {

#ifdef EQX_DEBUG
	void Print(std::string msg)
	{
		cout << msg << endl;
	}

	void Print(int num)
	{
		cout << num << endl;
	}

	void Print(unsigned int num)
	{
		cout << num << endl;
	}

	void Print(size_t num)
	{
		cout << num << endl;
	}

	void Print(float num)
	{
		cout << num << endl;
	}

	void Print(double num)
	{
		cout << num << endl;
	}

	void Print(const Vector3& vec)
	{
		cout << vec.x << " " << vec.y << " " << vec.z << " " << endl;
	}

	void Print(const Vector4& vec)
	{
		cout << vec.x << " " << vec.y << " " << vec.z << " " << endl;
	}

	void Print(const Face& f)
	{
		cout << "| Face:" << endl;
		cout << "| Vertex 1: ";
		Print(f.L().pos);
		cout << "| Vertex 2: ";
		Print(f.M().pos);
		cout << "|_Vertex 3: ";
		Print(f.R().pos);
	}
#endif

}
