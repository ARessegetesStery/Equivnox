#pragma once

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
	};

	using LineSegs = std::vector<LineSeg>;

	float P2LDistance(LineSeg& l, Vertex p);

	float PixelAmp(LineSeg&, Vertex);

	float GetSlope(Vertex& v1, Vertex& v2);

	// For Line Rendering
	const float SLOPE_MAX = 1000;

}