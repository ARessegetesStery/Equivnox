#pragma once

#include <CoreMinimal.h>

#include "Vertex.h"

namespace EQX {

	class LineSeg
	{
	public: 
		Vertex start;
		Vertex end;
		XFloat k;
		XInt kSign;

	public: 
		LineSeg();
		LineSeg(Vertex s, Vertex e);

		void Transform(const Mat4& projection);
		void Transform(const MeshTransform& trans);
	};

	XFloat P2LDistance(LineSeg& l, Vertex p);

	XFloat PixelAmp(LineSeg&, Vertex);

	XFloat GetSlope(Vertex& v1, Vertex& v2);

}