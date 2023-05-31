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
	// Instantiate an entity using reference
	EQX::Renderer& coreRenderer = EQX::Renderer::Init();

	// EQX::Vertex v1(EQX::Vector2(147, 161)), v2(EQX::Vector2(158, 231)), v3(EQX::Vector2(177, 267));
	// std::array<EQX::Vertex, 3> vertices{ EQX::Vector2(10, 80), EQX::Vector2(40, 90), EQX::Vector2(40, 140) };
	// EQX::Face f(v1, v2, v3);

	// m.AddLine(v1, v3);
	// m.AddFace(f);

	// EQX::Mesh m1(m);
	// EQX::Scene defaultScene;
	// EQX::Entity ent(m);
	// m1.Shift(EQX::Vector3(0, 0, 0.5));
	// EQX::Entity ent1(m1);
	EQX::MeshTransform trans;
	trans.displacement = EQX::Vec3(0, 0, 0.5);

	EQX::MeshTransform scale;
	scale.scaleCoeff = EQX::Vec3(0.3, 0.3, 0.3);

	// TODO Further simplify with parent in EntityInfo
	EQX::SceneInfo testScene = coreRenderer.CreateEmptyScene("test");
	EQX::EntityInfo cube1 = coreRenderer.CreateEmptyEntityUnderScene(testScene, "cube1");
	EQX::EntityInfo cube2 = coreRenderer.DuplicateEntityWithTransform(testScene, "cube1", "cube2", trans);

	EQX::ObjParser objParser(coreRenderer, cube1, "Cube");
	objParser.ParseWith(scale);

	coreRenderer.BindScene(testScene);

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

	// coreRenderer.SetWidthScale(2.5);
	// coreRenderer.SetHeightScale(2.5);
	// coreRenderer.DisableCamera();

	coreRenderer.addLight(EQX::Light(coreRenderer.camera.GetPos(), EQX::Color::White, .5f, EQX::LightType::Point));

	coreRenderer.Render();
}


