#pragma once

#include <CoreMinimal.h>

#include "AssetManager.h"
#include "Preprocess.h"
#include "RenderConfig.h"
#include "Runtime/ExternalTools/ExternalTools.h"
#include "Runtime/Geometry/Geometry.h"
#include "Runtime/Landscape/Landscape.h"

namespace EQX
{
	using MSAABuffer = ImageBuffer<EQXChar, EQXInt, 0>;
	using LightZBuffer = ImageBuffer<EQXFloat, EQXInt, 0>;

	/**
	 * 
	 */
	class Renderer
	{
		/*  Friends here are toolchains that will be invoked by Renderer  */
		friend class ObjParser;
	public:
		// canvas size should be determined upon initialization to avoid reallocation of memory
		static Renderer& Init();
		static Renderer& Init(EQXUInt, EQXUInt);

		void BindScene(SceneInfo);
		void UnbindScene();

		void Render();

		/*  Settings and Querying  */
		inline void SetFill(RenderFill f) { this->renderFill = f; }
		inline void SetPass(RenderPass p) { this->renderPass = p; }
		inline void SetAA(RenderAAConfig t) { this->renderAAConfig = t; }
		inline void SetLight(ShadingMode t) { this->renderLightConfig = t; }
		inline void SetOutputType(ImageType t) { this->imageType = t; }
		inline void SetOutputPath(std::string s) { this->outputPath = s; }
		
		inline void SetWidthScale(EQXFloat w) { this->wScale = w; }
		inline void SetHeightScale(EQXFloat h) { this->hScale = h; }
		void SetMSAAMult(EQXUInt);

		// Accept pixel info and render as it is
		inline void DisableCamera() 
		{ 
			this->cameraEnabled = false;
			// cannot render ZBuffer when camera is disabled
			this->renderPass = RenderPass::FULL;
		}		
		inline void EnableCamera() { this->cameraEnabled = true; }			// Enable rendering with perspective

		void AddLight(Light);

		/*  Interacting with Asset Manager  */
		SceneInfo CreateEmptyScene(std::string sceneName);
		EntityInfo CreateEmptyEntityUnderScene(SceneInfo scene, std::string entityName);
		EntityInfo DuplicateEntity(SceneInfo curScene, std::string from, std::string to);
		EntityInfo DuplicateEntityWithTransform(SceneInfo curScene, std::string from, std::string to, const MeshTransform& trans);

		/*  Setting Main Camera  */
		inline void fromFoV(const EQXFloat FoV, const EQXFloat aspect) { this->camera.fromFoV(FoV, aspect); };
		inline void SetCameraPos(Vec4 pos) { this->camera.SetPos(pos); }
		inline void SetCameraPos(Vec3 pos) { this->camera.SetPos(pos); }
		inline void CameraLookAt(Vec3 pos) { this->camera.LookAt(pos); }
		inline void CameraLookAt(Vec4 pos) { this->camera.LookAt(pos); }
		inline void SetCameraUp(Vec4 up) { this->camera.SetUp(up); }
		inline void SetCameraUp(Vec3 up) { this->camera.SetUp(up); }
		inline Vec3 GetCameraPos() { return this->camera.GetPos(); }

	private:
		/*  Scene Raw Data  */
		Camera camera;
		std::vector<Light> lights;

		Renderer();
		Renderer(EQXUInt, EQXUInt);
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		/*  Main Render Processes  */
		void Rasterize();
		void Raytrace();
		void RenderLines();
		void RenderFaces();

		/// No need to clip for line rendering
		void RenderLineRaw(const LineSeg&);
		void RenderLineSmooth(const LineSeg&);

		void RenderZBuf(std::vector<EntityConfig>::iterator entConfig, Scene& sceneToRender);
		void RenderLightZBuf(); // TODO

		void RenderFaceSingle(const Face& fOriginal, const Face& fTransformed);
		void RenderFaceZBuf(const Face& fTransformed);

		void UpdateZBufColor(EQXFloat x, EQXFloat y, const Face& f);

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
		void UpdateFragColor(EQXFloat x, EQXFloat y, const Face& f, const Face& fOriginal);

		Color PhongLighting(Vec3 originalPos, Vec3 fragNormal, Color texColor, const Light& l);

		/*  Render Configs  */
		EQXBool cameraEnabled;
		EQXBool hardShadow;
		RenderFill renderFill;
		RenderPass renderPass;
		RenderAAConfig renderAAConfig;
		ShadingMode renderLightConfig;
		EQXUInt MSAAMult;			// the rate of sampling in MSAA, 2^x
		Scene* curScene;

		EQXUInt width, height;
		EQXFloat wScale, hScale;
		ImageType imageType;
		std::string outputPath;

		/*  Asset Management  */
		AssetManager& assetManager;

		/*  Temporary Data Storage  */
		Mat4 perspTransform;					  // Perspective Transform Matrix
		Mat4 ssTransform;						  // Screenspace Transform Matrix
		Mat4 transform;							  // Transform Matrix; = persp * ss
		Mat4 inverseTransform;					  // Inverse Transform Matrix
		std::vector<Mat4> lightPerspTransforms;	  // Transform Matrices for lights, same sequence as that of creation

		/*  Buffers and Outputs  */
		Image outputImage;
		ImageGrey ZBuffer;
		MSAABuffer MSAAMask;
		std::vector<LightZBuffer> lightZMaps;

		/*  Helper Functions  */
		EQXBool Validify();
		void ValidateConfig();		// Ensures no config conflict exists

		Mat4 perspMatFromCamera(Camera& c);

		/*  Tools that may be Invoked by Friends  */
		Scene& _scene(std::string sceneName);
		Scene& _scene(SceneInfo sceneInfo);

		EntityConfig& _configUnderScene(SceneInfo scene, std::string entName);

		Entity& _entityUnderScene(SceneInfo sceneInfo, EntityID id);

		void FrustumClippingWithCamera();
	}; 


}
