#pragma once

#include "eqxpch.h"

#include "vendor/tgaimage.h"
#include "Math/MathHeader.h"
#include "Basics/Renderer/Core/Core.h"

namespace EQX {

	class LineSeg
	{
	public: 
		Vertex start;
		Vertex end;
		double k;
		double kSign;

	public: 
		LineSeg();
		LineSeg(Vertex s, Vertex e);
		
		void render(TGAImage&, TGAColor);
	};

	double P2LDistance(LineSeg& l, Vertex p);

	double pixelAmp(LineSeg&, Vertex);

}