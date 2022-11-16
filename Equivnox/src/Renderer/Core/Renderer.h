#pragma once

#include "eqxpch.h"

#include "RenderConfig.h"
#include "Renderer/Model/Model.h"
#include "Renderer/Utility/Utilities.h"

// TODO optimize: whether construct line/shape entities

namespace EQX
{
	class Renderer
	{
	public:
		static std::shared_ptr<Renderer> Init();

		void bindMesh(Mesh*);
		void unbindMesh();

		void render();

		void setFill(RenderFill);
		void setMode(RenderMode);

	private:
		Renderer();
		Renderer(const Renderer& r) = delete;
		Renderer& operator= (const Renderer& r) = delete;

		static std::shared_ptr<Renderer> p_renderer;

		Mesh* curMesh;

		// TODO decode line arrays
		void renderLineRaw(TGAImage&);
		void renderLineSmooth(TGAImage&);

		RenderFill fill;
		RenderMode mode;

		std::string outputPath;

	};

	

}
