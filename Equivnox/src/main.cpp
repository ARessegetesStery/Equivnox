#include "eqxpch.h"

#include <CoreDebugs.h>

#include "Runtime/Renderer/Core.h"
#include "Runtime/Geometry/Geometry.h"
#include "Loader/Loader.h"
#include "Runtime/Landscape/Landscape.h"
#include "Runtime/ExternalTools/Image.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}

int main(int argc, char** argv) {
	EQX::Renderer& coreRenderer = EQX::Renderer::Init();

	EQX::MeshTransform trans;
	trans.displacement = EQX::Vec3(0, 0, 0.6f);

	EQX::MeshTransform scale;
	scale.scaleCoeff = EQX::Vec3(0.3f, 0.3f, 0.3f);

	// TODO Further simplify with parent in EntityInfo
	EQX::SceneInfo testScene = coreRenderer.CreateEmptyScene("test");
	EQX::EntityInfo cube1 = coreRenderer.CreateEmptyEntityUnderScene(testScene, "cube1");
	 
	EQX::ObjParser objParser(coreRenderer, cube1, "Cube");
	objParser.ParseWith(scale);

	EQX::EntityInfo cube2 = coreRenderer.DuplicateEntityWithTransform(testScene, "cube1", "cube2", trans);
	 
	coreRenderer.BindScene(testScene);

	coreRenderer.SetPass(EQX::RenderPass::FULL);
	coreRenderer.SetFill(EQX::RenderFill::FILL);
	coreRenderer.SetAA(EQX::RenderAAConfig::MSAA);
	coreRenderer.SetMSAAMult(4);
	coreRenderer.SetOutputType(EQX::ImageType::PNG);
	coreRenderer.SetLight(EQX::ShadingMode::RASTERIZE);

	coreRenderer.EnableCamera();
	coreRenderer.SetCameraPos(EQX::Vec3(0, 1, 2));
	coreRenderer.CameraLookAt(EQX::Vec3::Zero);
	coreRenderer.fromFoV(45, 1.0);

	coreRenderer.AddLight(EQX::Light(coreRenderer.GetCameraPos(), EQX::Color::White, .5f, EQX::LightType::Point));

	coreRenderer.Render();
}


