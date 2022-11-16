#include "eqxpch.h"

#include "Basics/Renderer/Core/Core.h"

// #include "tgaimage.h"
// #include "Basics/Shapes/LineSeg.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}


int main(int argc, char** argv) {
	/*
	std::string path = "output.tga";

	TGAImage image(100, 100, TGAImage::RGB);

	EQX::LineSeg l2(EQX::Vector2(10, 80), EQX::Vector2(20, 30));
	EQX::LineSeg l(EQX::Vector2(10, 20), EQX::Vector2(10, 30));

	l.render(image, EQX::red);
	l2.render(image, EQX::white);
	image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
	image.write_tga_file(path.c_str());
	*/

	std::shared_ptr<EQX::Renderer> coreRenderer = EQX::Renderer::Init();
	EQX::LineSeg l(EQX::Vector2(10, 80), EQX::Vector2(20, 30));
	EQX::Mesh m;
	m.addLine(l);
	coreRenderer->bindMesh(&m);
	coreRenderer->render();
}


