#include "eqxpch.h"

#include "tgaimage.h"
#include "Config/config.h"
#include "Basics/Shapes/line.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}


int main(int argc, char** argv) {
	std::string path = "output.tga";

	TGAImage image(100, 100, TGAImage::RGB);

	EQX::line l(EQX::vec2(10, 20), EQX::vec2(10, 30));
	EQX::line l2(EQX::vec2(10, 20), EQX::vec2(40, 30));
	std::cout << l2.k << std::endl;

	l.render(image, EQX::red);
	l2.render(image, EQX::white);
	image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
	image.write_tga_file(path.c_str());
}


