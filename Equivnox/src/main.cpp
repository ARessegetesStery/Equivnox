#include "eqxpch.h"

#include "Renderer/Core/Core.h"

namespace EQX {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
}


int main(int argc, char** argv) {
	// Instantiate an entity using reference
	EQX::Renderer& coreRenderer = EQX::Renderer::Init();

	EQX::LineSeg l(EQX::Vector2(10, 80), EQX::Vector2(20, 30));
	EQX::LineSeg l2(EQX::Vector2(10, 80), EQX::Vector2(40, 70));
	EQX::LineSeg l3(EQX::Vector2(40, 60), EQX::Vector2(40, 170));
	EQX::LineSeg l4(EQX::Vector2(40, 60), EQX::Vector2(140, 60));
	EQX::Mesh m;
	m.addLine({ l, l2, l3, l4 });
	coreRenderer.bindMesh(&m);
	coreRenderer.setAA(EQX::RenderAAConfig::ANTIALIAS_ON);
	coreRenderer.render();
}


