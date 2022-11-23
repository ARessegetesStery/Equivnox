#include "eqxpch.h"

#include "Renderer/Core/Core.h"
#include "Renderer/Shapes/Shapes.h"
#include "Loader/Loader.h"

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

	EQX::Parser parser(&m, "cube");
	parser.parse();

	EQX::Vertex v1(EQX::Vector2(110, 80)), v2(EQX::Vector2(40, 290)), v3(EQX::Vector2(240, 140));

	std::array<EQX::Vertex, 3> vertices{ EQX::Vector2(10, 80), EQX::Vector2(40, 90), EQX::Vector2(40, 140) };
	EQX::Face f(v1, v2, v3);
	m.addFace(f);
	coreRenderer.BindMesh(&m);
	coreRenderer.SetFill(EQX::RenderFill::FILL);
	coreRenderer.SetAA(EQX::RenderAAConfig::ANTIALIAS_ON);
	coreRenderer.Render();

	/*
	EQX::Vertex v1(EQX::Vector2(10, 80)), v2(EQX::Vector2(40, 90)), v3(EQX::Vector2(40, 140));
	EQX::Face f(v1, v2, v3);
	TGAImage image(400, 400, TGAImage::RGB);
	cout << f.kLM << " " << f.kLR << " " << f.kMR << endl;
	f.Render(image);
	image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
	image.write_tga_file("output.tga");
	*/
}


