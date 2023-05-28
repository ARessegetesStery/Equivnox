#pragma once

#include "eqxpch.h"

#include "Preprocess.h"
#include "RenderConfig.h"
#include "Renderer/ExternalTools/ExternalTools.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Landscape/Landscape.h"

namespace EQX
{
	/**
	 * 
	 */
	class Renderer
	{
	public:
#ifdef EQX_DEBUG
		friend class Face;
#endif
		// canvas size should be determined upon initialization to avoid reallocation of memory
		static Renderer& Init();
		static Renderer& Init(unsigned int, unsigned int);

		void BindScene(Scene*);
		void UnbindScene();

		void Render();
		void Rasterize();
		void Raytrace();

		inline void SetFill(RenderFill f) { this->renderFill = f; }
		inline void SetPass(RenderPass p) { this->renderPass = p; }
		inline void SetAA(RenderAAConfig t) { this->renderAAConfig = t; }
		inline void SetLight(ShadingMode t) { this->renderLightConfig = t; }
		inline void SetOutputType(ImageType t) { this->imageType = t; }
		inline void SetOutputPath(std::string s) { this->outputPath = s; }
		
		inline void SetWidthScale(float w) { this->wScale = w; }
		inline void SetHeightScale(float h) { this->hScale = h; }
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
		Renderer(unsigned int, unsigned int);
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		/*  Main Render Processes  */
		void RenderLines();
		void RenderFaces();

		/// No need to clip for line rendering
		void RenderLineRaw(const LineSeg&);
		void RenderLineSmooth(const LineSeg&);

		void RenderFaceSingle(const Face& fOriginal, const Face& fTransformed);
		void RenderFaceZBuf(const Face& fTransformed);

		/*  Render Configs  */
		bool cameraEnabled;
		bool hardShadow;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		ShadingMode renderLightConfig;
		unsigned int MSAAMult;			// the rate of sampling in MSAA, 2^x
		Scene* curScene;

		unsigned int width, height;
		float wScale, hScale;
		ImageType imageType;
		std::string outputPath;

		/*  Temporary Data Storage  */
		Mat4 perspTransform;			// Perspective Transform Matrix
		Mat4 ssTransform;				// Screenspace Transform Matrix
		Mat4 transform;					// Transform Matrix; = persp * ss
		Mat4 inverseTransform;			// Inverse Transform Matrix

		/*  Buffers and Outputs  */
		Image outputImage;
		ImageGrey ZBuffer;
		ImageMask<char, int, 0> MSAAMask;
		std::vector<ImageMask<float, int, 0>> lightZMaps;

		/*  Helper Functions  */
		void UpdateZBufColor(float x, float y, const Face& f);

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
		void UpdateFragColor(float x, float y, const Face& f, const Face& fOriginal);

		Color PhongLighting(Vec3 originalPos, Vec3 fragNormal, Color texColor, const Light& l);

		void ValidateConfig();		// Ensures no config conflict presents
	};


}
