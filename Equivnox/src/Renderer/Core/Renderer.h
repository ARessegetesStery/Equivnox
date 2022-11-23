#pragma once

#include "eqxpch.h"

#include "RenderConfig.h"
#include "Renderer/Shapes/Shapes.h"
#include "Renderer/Utility/Utilities.h"

// TODO optimize: whether construct line/shape entities

namespace EQX
{
	class Renderer
	{
	public:
#ifdef EQX_DEBUG
		friend class Face;
#endif
		static Renderer& Init();

		void BindMesh(Mesh*);
		void UnbindMesh();

		void Render();

		void SetFill(RenderFill);
		void SetMode(RenderMode);
		void SetAA(RenderAAConfig);
			 
		void SetCanvas(unsigned int, unsigned int);
		void SetCanvasHeight(unsigned int);
		void SetCanvasWidth(unsigned int);

	private:
		Renderer();
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		Mesh* curMesh;

		void RenderLines(TGAImage&);
		void RenderFaces(TGAImage&);

		void RenderLineRaw(TGAImage&, LineSeg);
		void RenderLineSmooth(TGAImage&, LineSeg);

		void RenderFaceRaw(TGAImage&, Face);

		RenderFill renderFill;
		RenderMode renderMode;
		RenderAAConfig renderAAConfig;

		unsigned int width, height;

		std::string outputPath;

	};

	

}
