#include "eqxpch.h"

#include "Renderer/Core/Core.h"
#include "Renderer/Shapes/Shapes.h"
#include "Loader/Loader.h"
#include "Renderer/ExternalTools/Image.h"

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
	m.Scale(0.7f);

	EQX::Vertex v1(EQX::Vector2(169, 58)), v2(EQX::Vector2(253, 306)), v3(EQX::Vector2(169, 152));

	std::array<EQX::Vertex, 3> vertices{ EQX::Vector2(10, 80), EQX::Vector2(40, 90), EQX::Vector2(40, 140) };
	EQX::Face f(v1, v2, v3);
	// m.Shift(EQX::Vector3(0, 120, 0));
	// m.AddLine(v1, v3);
	// m.AddFace(f);

	coreRenderer.BindMesh(&m);
	coreRenderer.SetFill(EQX::RenderFill::WIREFRAME);
	coreRenderer.SetAA(EQX::RenderAAConfig::MSAA);
	coreRenderer.SetOutputType(EQX::ImageType::TGA);

	coreRenderer.EnableCamera();
	coreRenderer.camera.pos = EQX::Vector3(0, 0, 2);
	coreRenderer.camera.lookAt = EQX::Vec3(0, 0, -1);
	coreRenderer.camera.fromFoV(45, 1.0);
	cout << coreRenderer.camera.width << " " << coreRenderer.camera.height << endl;
	coreRenderer.camera.upDir = EQX::Vec3(0, 1, 0);
	// coreRenderer.DisableCamera();

	coreRenderer.Render();

}


