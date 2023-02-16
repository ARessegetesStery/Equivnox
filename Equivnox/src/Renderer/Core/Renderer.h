#pragma once

#include "eqxpch.h"

#include "Renderer/ExternalTools/ExternalTools.h"
#include "Renderer/Geometry/Geometry.h"
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

		// Accept pixel info and render as it is
		inline void DisableCamera() 
		{ 
			this->cameraEnabled = false;
			// cannot render ZBuffer when camera is disabled
			this->renderPass = RenderPass::FULL;
		}		
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
		void RenderLines(EQX_OUT Image& image) const;
		void RenderFaces(EQX_OUT Image& image) const;

		/// No need to clip for line rendering
		void RenderLineRaw(EQX_OUT Image& image, LineSeg, const Mat4&) const;
		void RenderLineSmooth(EQX_OUT Image& image, LineSeg, const Mat4&) const;

		void RenderFaceSingle(EQX_OUT Image& image, const Face& fOriginal, const Face& fTransformed, const Image& ZBuffer) const;
		void RenderFaceZBuf(EQX_OUT ImageGrey& image, const Face& fTransformed) const;

		/*  Render Configs  */
		bool cameraEnabled;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		RenderLightConfig renderLightConfig;
		unsigned int MSAAMult;			// the rate of sampling in MSAA, 2^x

		unsigned int width, height;
		ImageType imageType;
		std::string outputPath;

		/*  Temporary Data Storage  */
		Mat4 perspTransform;			// Perspective Transform Matrix
		Mat4 ssTransform;				// Screenspace Transform Matrix
		Mat4 transform;					// Transform Matrix; = persp * ss
		Mat4 inverseTransform;			// Inverse Transform Matrix

		/*  Helper Functions  */
		void UpdateZBufColor(float x, float y, const Face& f, EQX_OUT ImageGrey& ZBuffer) const;

		/**
		 * Render face according to the precomputed ZBuffer
		 * Note: After clipping fOriginal does not change
		 *
		 * @param x
		 * @param y
		 * @param Face - after perspective transformation
		 * @param fOriginal - Face without perspective transformation
		 * @param image - [OUT]Image to write to
		 * @param ZBuffer - Precomputed ZBuffer
		 */
		void UpdateFragColor(float x, float y, const Face& f, const Face& fOriginal, EQX_OUT Image& image, const Image& ZBuffer) const;

		Color PhongLighting(Vec3 originalPos, Vec3 fragNormal, Color texColor, const Light& l) const;

		void ValidateConfig();		// Ensures no config conflict presents
	};


}
