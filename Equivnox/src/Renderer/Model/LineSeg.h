#pragma once

#include "vendor/tgaimage.h"
#include "Math/MathHeader.h"
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
		
		void render(TGAImage&, TGAColor);
	};

	float P2LDistance(LineSeg& l, Vertex p);

	float pixelAmp(LineSeg&, Vertex);

	// For Line Rendering
	const float SLOPE_MAX = 1000;

}