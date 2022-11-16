#include "eqxpch.h"

#include "Renderer.h"

namespace EQX
{
	std::shared_ptr<Renderer> Renderer::p_renderer = nullptr;

	Renderer::Renderer()
	{
		this->curMesh = nullptr;
		this->fill = RenderFill::WIREFRAME;
		this->mode = RenderMode::FULL;
		this->outputPath = "output.tga";
	}

	std::shared_ptr<Renderer> Renderer::Init()
	{
		if (Renderer::p_renderer == nullptr)
			Renderer::p_renderer = std::shared_ptr<Renderer>(new Renderer);
		return Renderer::p_renderer;
	}

	void Renderer::bindMesh(Mesh* m)
	{
		this->curMesh = m;
	}

	void Renderer::unbindMesh()
	{
		this->curMesh = nullptr;
	}

	void Renderer::setFill(RenderFill f)
	{
		this->fill = f;
	}

	void Renderer::setMode(RenderMode m)
	{
		this->mode = m;
	}

	void Renderer::render()
	{
		TGAImage image(100, 100, TGAImage::RGB);
		renderLineSmooth(image);
		image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
		image.write_tga_file(this->outputPath.c_str());
	}

	void Renderer::renderLineRaw(TGAImage& image)
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);

		LineSeg l(this->curMesh->vertices[0], this->curMesh->vertices[1]);

		int sx = static_cast<int>(l.start.pos.x);
		int sy = static_cast<int>(l.start.pos.y);
		int ex = static_cast<int>(l.end.pos.x);
		int ey = static_cast<int>(l.end.pos.y);

		bool transpose = false;
		if (abs(l.k) > 1)
		{
			std::swap(sx, sy);
			std::swap(ex, ey);
			if (sx > ex)
			{
				std::swap(sx, ex);
				std::swap(sy, ey);
			}
			transpose = true;
		}

		for (int x = sx; x != ex + 1; x += 1)
		{
			if (transpose)
			{
				int y = std::roundf(1 / l.k * (x - sx)) + sy;
				image.set(y, x, white);
			}
			else
			{
				int y = std::roundf(l.k * (x - sx)) + sy;
				image.set(x, y, white);
			}
		}
	}

	void Renderer::renderLineSmooth(TGAImage& image)
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);

		LineSeg l(this->curMesh->vertices[0], this->curMesh->vertices[1]);

		int sx = static_cast<int>(l.start.pos.x);
		int sy = static_cast<int>(l.start.pos.y);
		int ex = static_cast<int>(l.end.pos.x);
		int ey = static_cast<int>(l.end.pos.y);

		int xPace = 1;
		int yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
		{
			for (int x = sx; x != ex + xPace; x += xPace)
				image.set(x, sy, blendTGAColor(white, image.get(x, sy), 1.0));
		}
		else if (abs(l.k) > SLOPE_MAX)
		{
			for (int y = sy; y != ey + yPace; y += yPace)
				image.set(sx, y, blendTGAColor(white, image.get(sx, y), 1.0));
		}
		else
		{
			for (int x = sx; x != ex + xPace; x += xPace)
			{
				for (int y = (int)(sy + l.k * (x - sx)) - yPace;
					y != (int)(sy + l.k * (x - sx + 1) + yPace);
					y += yPace)
					// only traverse pixels that will possibly be rendered
				{
					Vector2 center(x + xPace / 2.0, y + yPace / 2.0);
					float coeff = pixelAmp(l, center);
#ifdef EQX_DEBUG
					if (coeff == 0)
						cout << center.x << " " << center.y << " "
						<< P2LDistance(l, center) << " " << coeff << endl;
#endif
					image.set(x, y, blendTGAColor(white, image.get(x, y), coeff));
				}
			}
		}
	}

}
