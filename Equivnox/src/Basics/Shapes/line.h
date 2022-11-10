#pragma once

#include "eqxpch.h"

#include "vendor/tgaimage.h"
#include "Math/matrix.h"

namespace EQX {

	class line
	{
	public: 
		vec2 start;
		vec2 end;
		double k;
		double kSign;

	public: 
		line();
		line(vec2 s, vec2 e);
		
		void render(TGAImage&, TGAColor);
	};

	double P2LDistance(line& l, vec2 p);

	double pixelAmp(line&, vec2);

}