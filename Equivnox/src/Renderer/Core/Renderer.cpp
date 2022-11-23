#include "eqxpch.h"

#include "Renderer.h"

namespace EQX
{
	Renderer::Renderer()
	{
		this->curMesh = nullptr;
		this->renderFill = RenderFill::WIREFRAME;
		this->renderMode = RenderMode::FULL;
		this->renderAAConfig = RenderAAConfig::ANTIALIAS_OFF;
		this->outputPath = "output.tga";
		this->width = 400;
		this->height = 400;
	}

	Renderer& Renderer::Init()
	{
		static Renderer _renderer;
		return _renderer;
	}

	void Renderer::BindMesh(Mesh* m)
	{
		this->curMesh = m;
	}

	void Renderer::UnbindMesh()
	{
		this->curMesh = nullptr;
	}

	void Renderer::SetFill(RenderFill f)
	{
		this->renderFill = f;
	}

	void Renderer::SetMode(RenderMode m)
	{
		this->renderMode = m;
	}

	void Renderer::SetAA(RenderAAConfig a)
	{
		this->renderAAConfig = a;
	}

	void Renderer::SetCanvas(unsigned int w, unsigned int h)
	{
		this->width = w;
		this->height = h;
	}

	void Renderer::SetCanvasHeight(unsigned int h)
	{
		this->height = h;
	}

	void Renderer::SetCanvasWidth(unsigned int w)
	{
		this->width = w;
	}

	void Renderer::Render()
	{
		TGAImage image(this->width, this->height, TGAImage::RGB);
		
		switch (renderFill)
		{
		case RenderFill::WIREFRAME:
			RenderLines(image);
			break;
		case RenderFill::FILL:
			RenderFaces(image);
			break;
		default:
			break;
		}

		image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
		image.write_tga_file(this->outputPath.c_str());
	}

	void Renderer::RenderLines(TGAImage& image)
	{
		for (auto iter = curMesh->lineIndices.begin();
			iter != curMesh->lineIndices.cend(); ++iter)
		{
			switch (this->renderAAConfig)
			{
			case RenderAAConfig::ANTIALIAS_OFF:
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]));
				break;
			case RenderAAConfig::ANTIALIAS_ON:
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]));
				break;
			}
		}
		for (auto iter = curMesh->faceIndices.begin();
			iter != curMesh->faceIndices.cend(); ++iter)
		{
			switch (this->renderAAConfig)
			{
			case RenderAAConfig::ANTIALIAS_OFF:
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]));
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]));
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[2]],
					curMesh->vertices[(*iter)[1]]));
				break;
			case RenderAAConfig::ANTIALIAS_ON:
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]));
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]));
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[2]],
					curMesh->vertices[(*iter)[1]]));
				break;
			}
		}
	}

	void Renderer::RenderFaces(TGAImage& image)
	{
		for (auto iter = curMesh->faceIndices.begin();
			iter != curMesh->faceIndices.cend(); ++iter)
		{
			std::array<Vertex, 3> vertices{ curMesh->vertices[(*iter)[0]], curMesh->vertices[(*iter)[1]] , curMesh->vertices[(*iter)[2]] };
			switch (this->renderAAConfig)
			{
			case RenderAAConfig::ANTIALIAS_OFF:
				cout << curMesh->vertices[(*iter)[0]].pos.x << " " << curMesh->vertices[(*iter)[0]].pos.y << endl;
				RenderFaceRaw(image, vertices);
				break;
			case RenderAAConfig::ANTIALIAS_ON:
				RenderFaceRaw(image, vertices);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]));
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]));
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[2]],
					curMesh->vertices[(*iter)[1]]));
				break;
			}
		}
	}

	void Renderer::RenderLineRaw(TGAImage& image, LineSeg l)
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);

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

	void Renderer::RenderLineSmooth(TGAImage& image, LineSeg l)
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);
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
					Vector2 center(x + xPace / 2.0f, y + yPace / 2.0f);
					float coeff = PixelAmp(l, center);
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

	void Renderer::RenderFaceRaw(TGAImage& image, Face f)
	{
		cout << f.l.pos.x << f.l.pos.y << f.m.pos.x << f.m.pos.y << f.r.pos.x << f.r.pos.y;
		if (std::abs(f.kLM) > SLOPE_MAX)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos < f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.l.pos.x) * f.kMR; ++ypos)
				{
					image.set(xpos, ypos, TGAWhite);
				}
			}
		}
		else if (f.m.pos.y > f.r.pos.y)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos <= f.m.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLM; ++ypos)
				{
					image.set(xpos, ypos, TGAWhite);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.m.pos.x) * f.kMR; ++ypos)
				{
					image.set(xpos, ypos, TGAWhite);
				}
			}
		}
		else
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos <= f.m.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLM;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLR; ++ypos)
				{
					image.set(xpos, ypos, TGAWhite);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.m.pos.y + (xpos - f.m.pos.x) * f.kMR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLR; ++ypos)
				{
					image.set(xpos, ypos, TGAWhite);
				}
			}
		}
	}

}
