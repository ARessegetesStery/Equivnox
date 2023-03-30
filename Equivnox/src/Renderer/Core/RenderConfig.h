#pragma once

#include "Renderer/ExternalTools/ExternalTools.h"

namespace EQX {

	enum class RenderFill
	{
		WIREFRAME, FILL
	};

	enum class RenderPass
	{
		FULL, ZBUFFER_ONLY
	};

	enum class RenderAAConfig
	{
		ANTIALIAS_OFF, 
		SMOOTH,			// render raw shape + boundary lines
		MSAA, 
		FXAA
	};

	enum class RenderColorConfig
	{
		UNIFORM, VERTEX, TEXTURE
	};

	enum class ShadingMode
	{
		RASTERIZE, RAYTRACING
	};

#ifdef EQX_DEBUG
	const TGAColor TGAWhite = TGAColor(255, 255, 255, 255);
	const TGAColor TGANone = TGAColor(0, 0, 0, 255);
#endif

	class Renderer;

}