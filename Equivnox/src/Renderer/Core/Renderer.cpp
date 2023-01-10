#include "eqxpch.h"

#include "Renderer.h"

namespace EQX
{
	Renderer::Renderer()
	{
		this->curMesh = nullptr;
		this->cameraEnabled = true;
		this->renderFill = RenderFill::WIREFRAME;
		this->renderPass = RenderPass::FULL;
		this->renderAAConfig = RenderAAConfig::ANTIALIAS_OFF;
		this->renderLightConfig = RenderLightConfig::PHONG;
		this->imageType = ImageType::TGA;
		this->outputPath = "output.tga";
		this->width = 400;
		this->height = 400;
		this->MSAAMult = 2;
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

	void Renderer::SetPass(RenderPass m)
	{
		this->renderPass = m;
	}

	void Renderer::SetAA(RenderAAConfig a)
	{
		this->renderAAConfig = a;
	}

	void Renderer::SetLight(RenderLightConfig l)
	{
		this->renderLightConfig = l;
	}

	void Renderer::SetOutputType(ImageType t)
	{
		this->imageType = t;
	}

	void Renderer::SetMSAAMult(unsigned int scale)
	{
		if (scale == 0 || scale == 1)
			MSAAMult = 2;
		else if (scale > 6)
			MSAAMult = 6;
		else
			MSAAMult = scale;
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

	void Renderer::addLight(Light l)
	{
		this->lights.push_back(l);
	}

	void Renderer::ValidateConfig()
	{
		if (!this->cameraEnabled)
			this->renderPass = RenderPass::FULL;
	}

	void Renderer::Render()
	{
		Image image(this->width, this->height, this->imageType);

		/*  Initializing the background color to be black  */
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				image.set(x, y, Color::Black);

		/*  Setup Projection  */
		Mat4 PerspMat = makeScreenSpace(this->width, this->height) *
			makePersp(camera.width, camera.height) * makeView(camera.pos, camera.lookAt, camera.upDir);
		if (this->cameraEnabled)
			this->projection = PerspMat;
		else
			this->projection = Mat4::IDENTITY;
		this->inverseProjection = this->projection.Inverse();

		/*  Render Meshes  */
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

		image.write();
	}

	void Renderer::RenderLines(Image& image)
	{
		Mat4 PerspMat = makeScreenSpace(this->width, this->height) *
			makePersp(camera.width, camera.height, -camera.nearClip, -camera.farClip) * 
			makeView(camera.pos, camera.lookAt, camera.upDir);

		Mat4 Projection = Mat4::IDENTITY;
		if (this->cameraEnabled)
			Projection = PerspMat;

		for (auto iter = curMesh->lineIndices.begin();
			iter != curMesh->lineIndices.cend(); ++iter)
		{
			switch (this->renderAAConfig)
			{
			case RenderAAConfig::ANTIALIAS_OFF:
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]), Projection);
				break;
			case RenderAAConfig::MSAA: case RenderAAConfig::SMOOTH:
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]), Projection);
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
					curMesh->vertices[(*iter)[1]]), Projection);
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]), Projection);
				RenderLineRaw(image, LineSeg(curMesh->vertices[(*iter)[2]],
					curMesh->vertices[(*iter)[1]]), Projection);
				break;
			case RenderAAConfig::MSAA: case RenderAAConfig::FXAA:
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]), Projection);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]), Projection);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[1]],
					curMesh->vertices[(*iter)[2]]), Projection);
				break;
			}
		}
	}

	void Renderer::RenderFaces(Image& image)
	{
		Image ZBuf(this->width, this->height, this->imageType);

		// Background of ZBuf should be white
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				ZBuf.set(x, y, Color::White);

		for (auto iter = curMesh->faceIndices.begin();
			iter != curMesh->faceIndices.cend(); ++iter)
		{
			std::array<Vertex, 3> vertices{ curMesh->vertices[(*iter)[0]],
				curMesh->vertices[(*iter)[1]] , curMesh->vertices[(*iter)[2]] };

			// If camera disabled, no need to render ZBuffer
			if (this->cameraEnabled)
				RenderFaceZBuf(ZBuf, vertices);
		}

		/*  Rendering Only ZBuffer  */
		if (this->renderPass == RenderPass::ZBUFFER_ONLY)
		{
			image = ZBuf;
			return;
		}

		/*  Rendering Full Image  */
		for (auto iter = curMesh->faceIndices.begin();
			iter != curMesh->faceIndices.cend(); ++iter)
		{
			std::array<Vertex, 3> vertices{ curMesh->vertices[(*iter)[0]], 
				curMesh->vertices[(*iter)[1]] , curMesh->vertices[(*iter)[2]] };
			Face face(vertices);

			/*  Back-Surface Culling  */
			if (vertices[0].normal.z < 0 && vertices[1].normal.z < 0 && vertices[2].normal.z < 0)
			{
				// cout << "Culled" << endl;
				continue;
			}

			// Render Each Fragment
			switch (this->renderAAConfig)
			{
			case RenderAAConfig::ANTIALIAS_OFF:
				RenderFaceRaw(image, face, ZBuf);
				break;
			case RenderAAConfig::MSAA:
				// TODO change into real MSAA
				RenderFaceRaw(image, face, ZBuf);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[1]]), projection);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[0]],
					curMesh->vertices[(*iter)[2]]), projection);
				RenderLineSmooth(image, LineSeg(curMesh->vertices[(*iter)[2]],
					curMesh->vertices[(*iter)[1]]), projection);
				break;
			}
		}
	}

	void Renderer::RenderLineRaw(Image& image, LineSeg l, const Mat4& Projection)
	{
		l.start = TransformVertexPos(Projection, l.start);
		l.end = TransformVertexPos(Projection, l.end);
		LineSeg perspL(l.start, l.end);

		int sx = static_cast<int>(perspL.start.pos.x);
		int sy = static_cast<int>(perspL.start.pos.y);
		int ex = static_cast<int>(perspL.end.pos.x);
		int ey = static_cast<int>(perspL.end.pos.y);

		bool transpose = false;
		if (abs(perspL.k) > 1)
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
				int y = std::roundf(1 / perspL.k * (x - sx)) + sy;
				image.set(y, x, Color::White);
			}
			else
			{
				int y = std::roundf(perspL.k * (x - sx)) + sy;
				image.set(x, y, Color::White);
			}
		}
	}
	 
	void Renderer::RenderLineSmooth(Image& image, LineSeg l, const Mat4& Projection)
	{
		l.start = TransformVertexPos(Projection, l.start);
		l.end = TransformVertexPos(Projection, l.end);
		LineSeg perspL(l.start, l.end);

		int sx = static_cast<int>(perspL.start.pos.x);
		int sy = static_cast<int>(perspL.start.pos.y);
		int ex = static_cast<int>(perspL.end.pos.x);
		int ey = static_cast<int>(perspL.end.pos.y);
		cout << sx << " " << sy << " " << ex << " " << ey << endl;

		int xPace = 1;
		int yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
		{
			for (int x = sx; x != ex + xPace; x += xPace)
			{
				image.set(x, sy, blendColor(Color::White, image.get(x, sy), 1.0));
			}
		}
		else if (abs(perspL.k) > SLOPE_MAX)
		{
			for (int y = sy; y != ey + yPace; y += yPace)
			{
				
				image.set(sx, y, blendColor(Color::White, image.get(sx, y), 1.0));
			}
		}
		else
		{
			
			for (int x = sx; x != ex; x += xPace)
			{
				for (int y = (int)(sy + perspL.k * (x - sx - 1)) - 2 * yPace;
					y != (int)(sy + perspL.k * (x - sx + 2) + 2 * yPace);
					y += yPace)
					// only traverse pixels that will possibly be rendered
				{
					Vector2 center(x + xPace / 2.0f, y + yPace / 2.0f);
					float coeff = PixelAmp(perspL, center);
					image.set(x, y, blendColor(Color::White, image.get(x, y), coeff));
				}
			}
		}
	}
	
	void Renderer::RenderFaceRaw(Image& image, Face f, const Image& ZBuffer)
	{
		// TODO (GPU) Change into edge function (cross product)
		Face original = f;
		f.l = TransformVertexPos(projection, f.l);
		f.m = TransformVertexPos(projection, f.m);
		f.r = TransformVertexPos(projection, f.r);
		f.ValidateSeq();

		if (std::abs(f.kLM) > SLOPE_MAX)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos < f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.l.pos.x) * f.kMR; ++ypos)
				{
					UpdateFragColor(xpos, ypos, f, original, image, ZBuffer);
				}
			}
		}
		else if (f.kLM > f.kLR)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos <= f.m.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLM; ++ypos)
				{
					UpdateFragColor(xpos, ypos, f, original, image, ZBuffer);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.m.pos.x) * f.kMR; ++ypos)
				{
					UpdateFragColor(xpos, ypos, f, original, image, ZBuffer);
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
					UpdateFragColor(xpos, ypos, f, original, image, ZBuffer);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.m.pos.y + (xpos - f.m.pos.x) * f.kMR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLR; ++ypos)
				{
					UpdateFragColor(xpos, ypos, f, original, image, ZBuffer);
				}
			}
		}
	}

	void Renderer::RenderFaceZBuf(Image& image, Face f)
	{
		f.l = TransformVertexPos(projection, f.l);
		f.m = TransformVertexPos(projection, f.m);
		f.r = TransformVertexPos(projection, f.r);
		f.ValidateSeq();

		if (std::abs(f.kLM) > SLOPE_MAX)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos < f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.l.pos.x) * f.kMR; ++ypos)
				{
					UpdateZBufColor(xpos, ypos, f, image);
				}
			}
		}
		else if (f.kLM > f.kLR)
		{
			int xpos, ypos;
			for (xpos = f.l.pos.x; xpos <= f.m.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLM; ++ypos)
				{
					UpdateZBufColor(xpos, ypos, f, image);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.l.pos.y + (xpos - f.l.pos.x) * f.kLR;
					ypos <= f.m.pos.y + (xpos - f.m.pos.x) * f.kMR; ++ypos)
				{
					UpdateZBufColor(xpos, ypos, f, image);
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
					UpdateZBufColor(xpos, ypos, f, image);
				}
			}
			for (xpos = f.m.pos.x; xpos <= f.r.pos.x; ++xpos)
			{
				for (ypos = f.m.pos.y + (xpos - f.m.pos.x) * f.kMR;
					ypos <= f.l.pos.y + (xpos - f.l.pos.x) * f.kLR; ++ypos)
				{
					UpdateZBufColor(xpos, ypos, f, image);
				}
			}
		}
	}

	void Renderer::UpdateZBufColor(float x, float y, const Face& f, Image& ZBuf)
	{
		int curGreyScale = ZBuf.get(x, y).r;

		float zpos = f.ZatXY(Vec2(x, y));
		int newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;

		if (newGreyScale > 255 || newGreyScale < 0)
			return;

		if (x == 169 && y == 167)
			cout << newGreyScale << endl << "<<<<<<<<<" << endl;

		if (newGreyScale < curGreyScale)
			ZBuf.set(x, y, Color(newGreyScale));
	}

	
	void Renderer::UpdateFragColor(float xpos, float ypos, const Face& f, const Face& fOriginal, Image& image, const Image& ZBuffer)
	{
		// If camera disabled, only render white pixels
		if (!this->cameraEnabled)
		{
			image.set(xpos, ypos, Color::White);
			return;
		}

		/*  Full Pixel Processing  */
		int curGreyScale = ZBuffer.get(xpos, ypos).r;

		float zpos = f.ZatXY(Vec2(xpos, ypos));
		float newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;

		if (newGreyScale > 255 || newGreyScale < 0)
			return;

		if (newGreyScale < curGreyScale + 1)
		{
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseProjection * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.l.normal +
				baryCoord[1] * fOriginal.m.normal + baryCoord[2] * fOriginal.r.normal;
			Color pixelColor = Color(0);
			if (this->renderLightConfig == RenderLightConfig::PHONG)
			{
				Color texColor = Color(200); // TODO Texture Reading
				for (auto l : this->lights)
				{
					if (l.lightType == LightType::Point)
					{
						float distance = (l.Position - originalPos).Norm();
						Vec3 lightDir = (l.Position - originalPos) / distance;
						Vec3 viewDir = (this->camera.pos.ToVec3() - originalPos).Normalize();
						Vector3 halfDir = (lightDir + viewDir).Normalize();
						Color ambient = 0.1 * texColor;
						Color diffuse = (LitColor(l.lightColor, texColor) * l.intensity) / (distance * distance) *
							std::max(0.f, Dot(fragNormal, lightDir));
						Color specular = l.lightColor * l.intensity / (distance * distance) *
							pow(std::max(0.f, Dot(fragNormal, halfDir)), 8);
						Color resultColor = ambient + diffuse + specular;
						pixelColor = pixelColor + resultColor;
					}
				}
				image.set(xpos, ypos, pixelColor);
			}
			else if (this->renderLightConfig == RenderLightConfig::PARTICLE)
				// TODO GI
				image.set(xpos, ypos, Color::White);
			else
				image.set(xpos, ypos, Color::White);
		}
	}

}
