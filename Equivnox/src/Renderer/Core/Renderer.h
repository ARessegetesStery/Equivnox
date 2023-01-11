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

		inline void SetFill(RenderFill f) { this->renderFill = f; }
		inline void SetPass(RenderPass p) { this->renderPass = p; }
		inline void SetAA(RenderAAConfig t) { this->renderAAConfig = t; }
		inline void SetLight(RenderLightConfig t) { this->renderLightConfig = t; }
		inline void SetOutputType(ImageType t) { this->imageType = t; }
		inline void SetOutputPath(std::string s) { this->outputPath = s; }
		
		inline void SetCanvas(unsigned int w, unsigned int h) { this->width = w; this->height = h; }
		inline void SetCanvasHeight(unsigned int h) { this->height = h; }
		inline void SetCanvasWidth(unsigned int w) { this->width = w; };
		void SetMSAAMult(unsigned int);

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
		void RenderFaceZBuf(ImageGrey& image, Face f);

		/*  Render Configs  */
		bool cameraEnabled;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		RenderLightConfig renderLightConfig;
		unsigned int MSAAMult;   // the rate of sampling in MSAA, 2^x

		unsigned int width, height;
		ImageType imageType;
		std::string outputPath;

		/*  Temporary Data Storage  */
		Mat4 projection;
		Mat4 inverseProjection;

		/*  Helper Functions  */
		void UpdateZBufColor(float x, float y, const Face& f, ImageGrey& ZBuffer);

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
