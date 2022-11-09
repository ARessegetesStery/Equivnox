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

	EQX::line l(EQX::vec2(0, 0), EQX::vec2(99, 99));
	//std::cout << l.start.x << l.start.y << l.end.x << l.end.y << std::endl;

	l.render(image, EQX::red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file(path.c_str());

	/*
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(0, 99, EQX::red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file(path.c_str());
	return 0;
	*/
}


