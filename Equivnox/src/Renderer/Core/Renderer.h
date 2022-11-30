#pragma once

#include "eqxpch.h"

#include "Renderer/ExternalTools/ExternalTools.h"
#include "Renderer/Shapes/Shapes.h"
#include "RenderConfig.h"
#include "Camera.h"

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
		void SetOutputType(ImageType);

		void SetMSAAMult(unsigned int);
			 
		void SetCanvas(unsigned int, unsigned int);
		void SetCanvasHeight(unsigned int);
		void SetCanvasWidth(unsigned int);

		Camera camera;

	private:
		Renderer();
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		Mesh* curMesh;


		void RenderLines(Image&);
		void RenderFaces(Image&);

		void RenderLineRaw(Image&, LineSeg);
		void RenderLineSmooth(Image&, LineSeg, const Mat4&);

		void RenderFaceRaw(Image&, Face);

		RenderFill renderFill;
		RenderMode renderMode;
		RenderAAConfig renderAAConfig;

		ImageType imageType;

		unsigned int width, height;

		unsigned int MSAAMult;   // the rate of sampling in MSAA, 2^x

		std::string outputPath;

	};

	

}
