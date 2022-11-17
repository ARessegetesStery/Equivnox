#pragma once

#include "Renderer/Utility/Utilities.h"

namespace EQX {

	enum RenderFill
	{
		WIREFRAME, FILL
	};

	enum RenderMode
	{
		FULL, ZBUFFER_ONLY
	};

	enum RenderAAConfig
	{
		ANTIALIAS_ON, ANTIALIAS_OFF
	};

	enum RenderColorConfig
	{
		UNIFORM, VERTEX, TEXTURE
	};

#ifdef EQX_DEBUG
	const TGAColor TGAWhite = TGAColor(0, 0, 0, 255);
	const TGAColor TGANone = TGAColor(0, 0, 0, 255);
#endif

	class Renderer;

}