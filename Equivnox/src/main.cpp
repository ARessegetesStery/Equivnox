#include "eqxpch.h"

#include "Util/Log/Log.h"
#include "Renderer/Core/Core.h"
#include "Renderer/Geometry/Geometry.h"
#include "Loader/Loader.h"
#include "Renderer/Landscape/Landscape.h"
#include "Renderer/ExternalTools/Image.h"
#include "Math/MathHeader.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}

int main(int argc, char** argv) {
	EQX::Renderer& coreRenderer = EQX::Renderer::Init();

	EQX::MeshTransform trans;
	trans.displacement = EQX::Vec3(0, 0, 1.0);

	EQX::MeshTransform scale;
	scale.scaleCoeff = EQX::Vec3(0.3, 0.3, 0.3);

	// TODO Further simplify with parent in EntityInfo
	// EQX::SceneInfo testScene = coreRenderer.CreateEmptyScene("test");
	// EQX::EntityInfo cube1 = coreRenderer.CreateEmptyEntityUnderScene(testScene, "cube1");
	// EQX::EntityInfo cube2 = coreRenderer.DuplicateEntityWithTransform(testScene, "cube1", "cube1", trans);
	// 
	// EQX::ObjParser objParser(coreRenderer, cube1, "Cube");
	// objParser.ParseWith(scale);
	// 
	// coreRenderer.BindScene(testScene);

	coreRenderer.SetPass(EQX::RenderPass::FULL);
	coreRenderer.SetFill(EQX::RenderFill::FILL);
	coreRenderer.SetAA(EQX::RenderAAConfig::MSAA);
	coreRenderer.SetMSAAMult(4);
	coreRenderer.SetOutputType(EQX::ImageType::TGA);
	coreRenderer.SetLight(EQX::ShadingMode::RASTERIZE);

	coreRenderer.EnableCamera();
	coreRenderer.camera.SetPos(EQX::Vec3(0, 1, 2));
	coreRenderer.camera.LookAt(EQX::Vec3::Zero);
	coreRenderer.camera.fromFoV(45, 1.0);

	coreRenderer.addLight(EQX::Light(coreRenderer.camera.GetPos(), EQX::Color::White, .5f, EQX::LightType::Point));

	coreRenderer.Render();
}


