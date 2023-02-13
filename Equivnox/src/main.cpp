#include "eqxpch.h"

#include "Renderer/Core/Core.h"
#include "Renderer/Geometry/Geometry.h"
#include "Loader/Loader.h"
#include "Renderer/ExternalTools/Image.h"
#include "Math/MathHeader.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}


int main(int argc, char** argv) {
	// Instantiate an entity using reference
	EQX::Renderer& coreRenderer = EQX::Renderer::Init();

	EQX::LineSeg l(EQX::Vector2(10, 80), EQX::Vector2(40, 90));
	EQX::LineSeg l2(EQX::Vector2(40, 90), EQX::Vector2(40, 140));
	EQX::LineSeg l3(EQX::Vector2(10, 80), EQX::Vector2(40, 140));
	EQX::LineSeg l4(EQX::Vector2(40, 60), EQX::Vector2(140, 60));
	EQX::LineSeg l5(EQX::Vector2(40, 60), EQX::Vector2(140, 180));
	EQX::LineSeg l6(EQX::Vector2(40, 60), EQX::Vector2(140, 80));
	EQX::Mesh m;

	EQX::ObjParser objParser(&m, "Cube");
	objParser.Parse();
	m.Scale(0.3f);

	EQX::Vertex v1(EQX::Vector2(147, 161)), v2(EQX::Vector2(158, 231)), v3(EQX::Vector2(177, 267));

	std::array<EQX::Vertex, 3> vertices{ EQX::Vector2(10, 80), EQX::Vector2(40, 90), EQX::Vector2(40, 140) };
	EQX::Face f(v1, v2, v3);
	m.Shift(EQX::Vector3(0, 0, 1.4));
	// m.AddLine(v1, v3);
	// m.AddFace(f);

	coreRenderer.BindMesh(&m);
	coreRenderer.SetPass(EQX::RenderPass::FULL);
	coreRenderer.SetFill(EQX::RenderFill::FILL);
	coreRenderer.SetAA(EQX::RenderAAConfig::ANTIALIAS_OFF);
	coreRenderer.SetOutputType(EQX::ImageType::TGA);
	coreRenderer.SetLight(EQX::RenderLightConfig::PHONG);

	coreRenderer.EnableCamera();
	coreRenderer.camera.pos = EQX::Vec3(0, 0, 2);
	coreRenderer.camera.lookAt = EQX::Vec3(0, 0, -1);
	coreRenderer.camera.fromFoV(45, 1.0);
	cout << coreRenderer.camera.width << " " << coreRenderer.camera.height << endl;
	coreRenderer.camera.upDir = EQX::Vec3(0, 1, 0);
	// coreRenderer.DisableCamera();

	coreRenderer.addLight(EQX::Light(coreRenderer.camera.pos, EQX::Color::White, 0.5f, EQX::LightType::Point));

	coreRenderer.Render();
}


