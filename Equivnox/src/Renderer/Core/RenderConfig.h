#pragma once

#include "Renderer/Utility/Utilities.h"

namespace EQX {

	enum class RenderFill
	{
		WIREFRAME, FILL
	};

	enum class RenderMode
	{
		FULL, ZBUFFER_ONLY
	};

	enum class RenderAAConfig
	{
		ANTIALIAS_ON, ANTIALIAS_OFF
	};

	enum class RenderColorConfig
	{
		UNIFORM, VERTEX, TEXTURE
	};

#ifdef EQX_DEBUG
	const TGAColor TGAWhite = TGAColor(255, 255, 255, 255);
	const TGAColor TGANone = TGAColor(0, 0, 0, 255);
#endif

	class Renderer;

}