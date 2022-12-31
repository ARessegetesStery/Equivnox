#pragma once

#include "eqxpch.h"

#include "Renderer/ExternalTools/ExternalTools.h"
#include "Renderer/Shapes/Shapes.h"
#include "RenderConfig.h"
#include "Preprocess.h"
#include "Camera.h"

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
		void SetPass(RenderPass);
		void SetAA(RenderAAConfig);
		void SetOutputType(ImageType);

		void SetMSAAMult(unsigned int);
			 
		void SetCanvas(unsigned int, unsigned int);
		void SetCanvasHeight(unsigned int);
		void SetCanvasWidth(unsigned int);

		inline void DisableCamera() { this->cameraEnabled = false; }		// Accept pixel info and render as it is
		inline void EnableCamera() { this->cameraEnabled = true; }		// Enable rendering with perspective

		Camera camera;

	private:
		Renderer();
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		Mesh* curMesh;

		/*  Main Render Processes  */
		void RenderLines(Image&);
		void RenderFaces(Image&);

		void RenderLineRaw(Image&, LineSeg, const Mat4&);
		void RenderLineSmooth(Image&, LineSeg, const Mat4&);

		void RenderFaceRaw(Image&, Face, const Mat4&);

		void RenderFaceZBuf(Image&, Face, const Mat4&);

		/*  Render Configs  */
		bool cameraEnabled;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		ImageType imageType;

		unsigned int width, height;
		unsigned int MSAAMult;   // the rate of sampling in MSAA, 2^x
		std::string outputPath;

		/*  Helper Functions  */
		void UpdateZBufColor(float, float, const Face&, Image&);

	};

	

}
