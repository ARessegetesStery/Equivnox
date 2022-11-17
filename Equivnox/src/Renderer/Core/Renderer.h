#pragma once

#include "eqxpch.h"

#include "RenderConfig.h"
#include "Renderer/Model/Model.h"
#include "Renderer/Utility/Utilities.h"

// TODO optimize: whether construct line/shape entities

namespace EQX
{
	class Renderer
	{
	public:
		static Renderer& Init();

		void bindMesh(Mesh*);
		void unbindMesh();

		void render();

		void setFill(RenderFill);
		void setMode(RenderMode);
		
		void setAA(RenderAAConfig);

		void setCanvas(unsigned int, unsigned int);
		void setCanvasHeight(unsigned int);
		void setCanvasWidth(unsigned int);

	private:
		Renderer();
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		Mesh* curMesh;

		// TODO decode line arrays
		void renderLineRaw(TGAImage&, LineSeg);
		void renderLineSmooth(TGAImage&, LineSeg);

		RenderFill fill;
		RenderMode mode;
		RenderAAConfig aaConfig;

		unsigned int width, height;

		std::string outputPath;

	};

	

}
