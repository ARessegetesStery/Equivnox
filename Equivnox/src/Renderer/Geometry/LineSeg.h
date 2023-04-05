#pragma once

#include "eqxpch.h"

#include "Vertex.h"

namespace EQX {

	class LineSeg
	{
	public: 
		Vertex start;
		Vertex end;
		float k;
		int kSign;

	public: 
		LineSeg();
		LineSeg(Vertex s, Vertex e);

		void Transform(const Mat4& projection);
		void Transform(const MeshTransform& trans);
	};

	using LineSegs = std::vector<LineSeg>;

	float P2LDistance(LineSeg& l, Vertex p);

	float PixelAmp(LineSeg&, Vertex);

	float GetSlope(Vertex& v1, Vertex& v2);

}