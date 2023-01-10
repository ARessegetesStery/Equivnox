#pragma once

#include "eqxpch.h"

#include "Renderer/ExternalTools/ExternalTools.h"
#include "Renderer/Shapes/Shapes.h"
#include "RenderConfig.h"
#include "Preprocess.h"
#include "Camera.h"
#include "Light.h"

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
		void SetLight(RenderLightConfig);
		void SetOutputType(ImageType);

		void SetMSAAMult(unsigned int);
		
		void SetCanvas(unsigned int, unsigned int);
		void SetCanvasHeight(unsigned int);
		void SetCanvasWidth(unsigned int);

		inline void DisableCamera() { this->cameraEnabled = false; }		// Accept pixel info and render as it is
		inline void EnableCamera() { this->cameraEnabled = true; }			// Enable rendering with perspective

		void addLight(Light);

		Camera camera;
		std::vector<Light> lights;

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

		void RenderFaceRaw(Image& image, Face f, const Image& ZBuffer);
		void RenderFaceZBuf(Image& image, Face f);

		/*  Render Configs  */
		bool cameraEnabled;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		RenderLightConfig renderLightConfig;
		ImageType imageType;

		unsigned int width, height;
		unsigned int MSAAMult;   // the rate of sampling in MSAA, 2^x
		std::string outputPath;

		/*  Temporary Data Storage  */
		Mat4 projection;
		Mat4 inverseProjection;

		/*  Helper Functions  */
		void UpdateZBufColor(float x, float y, const Face& f, Image& ZBuffer);

		/**
		 * Render face according to the precomputed ZBuffer
		 *
		 * @param x
		 * @param y
		 * @param Face - after perspective transformation
		 * @param fOriginal - Face without perspective transformation
		 * @param image - Image to write to
		 * @param ZBuffer - Precomputed ZBuffer
		 */
		void UpdateFragColor(float x, float y, const Face& f, const Face& fOriginal, Image& image, const Image& ZBuffer);

		void ValidateConfig();		// Ensures no config conflict presents
	};

	

}
