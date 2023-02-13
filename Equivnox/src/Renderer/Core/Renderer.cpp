#include "eqxpch.h"

#include "Renderer.h"

namespace EQX
{
	Renderer::Renderer()
	{
		/*  Configurations  */
		this->curMesh = nullptr;
		this->cameraEnabled = true;
		this->renderFill = RenderFill::WIREFRAME;
		this->renderPass = RenderPass::FULL;
		this->renderAAConfig = RenderAAConfig::ANTIALIAS_OFF;
		this->renderLightConfig = RenderLightConfig::PHONG;
		this->imageType = ImageType::TGA;
		this->outputPath = "output";
		this->ssTransform = Mat4::Identity;
		this->perspTransform = Mat4::Identity;
		this->transform = Mat4::Identity;
		this->inverseTransform = Mat4::Identity;
		this->width = 400;
		this->height = 400;
		this->MSAAMult = 4;
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

	void Renderer::SetMSAAMult(unsigned int scale)
	{
		if (scale == 0 || scale == 1)
			MSAAMult = 2;
		else if (scale > 8)
			MSAAMult = 8;
		else
			MSAAMult = scale;
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
		Image image(this->width, this->height);

		/*  Initializing the background color to be black  */
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				image.set(x, y, Color::Black);

		/*  Setup Transformation Matrix  */
		Mat4 ssMat = MakeScreenSpace(this->width, this->height);
		Mat4 perspMat = MakePersp(camera.width, camera.height) * MakeView(camera.pos, camera.lookAt, camera.upDir);
		if (this->cameraEnabled)
		{
			this->ssTransform = ssMat;
			this->perspTransform = perspMat;
			this->transform = ssMat * perspMat;
			this->inverseTransform = this->transform.Inverse();
		}

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

		image.write(this->imageType, this->outputPath);
	}

	void Renderer::RenderLines(EQX_OUT Image& image) const
	{
		Mat4 PerspMat = MakeScreenSpace(this->width, this->height) *
			MakePersp(camera.width, camera.height, -camera.nearClip, -camera.farClip) * 
			MakeView(camera.pos, camera.lookAt, camera.upDir);

		Mat4 Projection = Mat4::Identity;
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

	void Renderer::RenderFaces(EQX_OUT Image& image) const
	{
		ImageGrey ZBuf(this->width, this->height);

		// Background of ZBuf should be white
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				ZBuf.set(x, y, Color::White);

		for (auto iter = curMesh->faceIndices.begin();
			iter != curMesh->faceIndices.cend(); ++iter)
		{
			std::array<Vertex, 3> vertices{ curMesh->vertices[(*iter)[0]],
				curMesh->vertices[(*iter)[1]] , curMesh->vertices[(*iter)[2]] };
			Face fOriginal(vertices);
			Face fTransformed(fOriginal);
			TransformFace(fTransformed, transform);

			// If camera disabled, no need to render ZBuffer
			if (this->cameraEnabled)
				RenderFaceZBuf(ZBuf, fTransformed);
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
			Face fOriginal(vertices);
			Face fTransformed(fOriginal);
			TransformFace(fTransformed, perspTransform);

			/*  Back-Surface Culling  */
			if (vertices[0].normal.z < 0 && vertices[1].normal.z < 0 && vertices[2].normal.z < 0)
				continue;

			/*  Render Each Fragment with Clipping  */
			std::vector<Face> fsClipped;
			if (FrustumClipping(fTransformed, fsClipped))
				for (auto& fClipped : fsClipped)
					RenderFaceSingle(image, fOriginal, fClipped, ZBuf);
		}
	}

	void Renderer::RenderLineRaw(EQX_OUT Image& image, LineSeg l, const Mat4& Projection) const
	{
		TransformVertexPos(Projection, l.start);
		TransformVertexPos(Projection, l.end);
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
	 
	void Renderer::RenderLineSmooth(EQX_OUT Image& image, LineSeg l, const Mat4& Projection) const
	{
		TransformVertexPos(Projection, l.start);
		TransformVertexPos(Projection, l.end);
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
			for (int x = sx; x != ex + xPace; x += xPace)
				image.set(x, sy, blendColor(Color::White, image.get(x, sy), 1.0));
		else if (abs(perspL.k) > SLOPE_MAX)
			for (int y = sy; y != ey + yPace; y += yPace)
				image.set(sx, y, blendColor(Color::White, image.get(sx, y), 1.0));
		else
		{
			for (int x = sx; x != ex; x += xPace)
			{
				for (int y = (int)(sy + perspL.k * (x - sx - 1)) - 2 * yPace;
					y != (int)(sy + perspL.k * (x - sx + 2) + 2 * yPace);
					y += yPace)
					// only traverse pixels that will possibly be rendered
				{
					if (x >= this->width || y >= this->height)
						continue;
					Vector2 center(x + xPace / 2.0f, y + yPace / 2.0f);
					float coeff = PixelAmp(perspL, center);
					image.set(x, y, blendColor(Color::White, image.get(x, y), coeff));
				}
			}
		}
	}
	
	void Renderer::RenderFaceSingle(EQX_OUT Image& image, const Face& fOriginal, Face& fTransformed, const Image& ZBuffer) const
	{
		TransformFace(fTransformed, ssTransform);

		if (std::abs(fTransformed.kLM) > SLOPE_MAX)
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos < fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR;
					ypos <= fTransformed.m.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kMR; ++ypos)
					UpdateFragColor(xpos, ypos, fTransformed, fOriginal, image, ZBuffer);
		}
		else if (fTransformed.kLM > fTransformed.kLR)
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos < floor(fTransformed.m.pos.x); ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR - 1;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLM + 1; ++ypos)
					UpdateFragColor(xpos, ypos, fTransformed, fOriginal, image, ZBuffer);
			for (xpos = fTransformed.m.pos.x; xpos <= fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR - 1;
					ypos <= fTransformed.m.pos.y + (xpos - fTransformed.m.pos.x) * fTransformed.kMR + 1; ++ypos)
					UpdateFragColor(xpos, ypos, fTransformed, fOriginal, image, ZBuffer);
		}
		else
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos < floor(fTransformed.m.pos.x); ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLM - 1;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR + 1; ++ypos)
					UpdateFragColor(xpos, ypos, fTransformed, fOriginal, image, ZBuffer);
			for (xpos = fTransformed.m.pos.x; xpos <= fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.m.pos.y + (xpos - fTransformed.m.pos.x) * fTransformed.kMR - 1;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR + 1; ++ypos)
					UpdateFragColor(xpos, ypos, fTransformed, fOriginal, image, ZBuffer);
		}
	}

	void Renderer::RenderFaceZBuf(EQX_OUT ImageGrey& image, const Face& fTransformed) const
	{
		if (std::abs(fTransformed.kLM) > SLOPE_MAX)
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos < fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR;
					ypos <= fTransformed.m.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kMR; ++ypos)
					UpdateZBufColor(xpos, ypos, fTransformed, image);
		}
		else if (fTransformed.kLM > fTransformed.kLR)
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos <= fTransformed.m.pos.x; ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLM; ++ypos)
					if (image.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTransformed, image);
			for (xpos = fTransformed.m.pos.x; xpos <= fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR;
					ypos <= fTransformed.m.pos.y + (xpos - fTransformed.m.pos.x) * fTransformed.kMR; ++ypos)
					if (image.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTransformed, image);
		}
		else
		{
			int xpos, ypos;
			for (xpos = fTransformed.l.pos.x; xpos <= fTransformed.m.pos.x; ++xpos)
			{
				for (ypos = fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLM;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR; ++ypos)
					if (image.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTransformed, image);
			}
			for (xpos = fTransformed.m.pos.x; xpos <= fTransformed.r.pos.x; ++xpos)
				for (ypos = fTransformed.m.pos.y + (xpos - fTransformed.m.pos.x) * fTransformed.kMR;
					ypos <= fTransformed.l.pos.y + (xpos - fTransformed.l.pos.x) * fTransformed.kLR; ++ypos)
					if (image.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTransformed, image);
		}
	}

	void Renderer::UpdateZBufColor(float x, float y, const Face& f, EQX_OUT ImageGrey& ZBuf) const
	{
		int curGreyScale = ZBuf.get(x, y).r;

		float zpos = f.ZAtXYFace(Vec2(x, y));
		int newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;

		if (newGreyScale > 255 || newGreyScale < 0)
			return;

		if (newGreyScale < curGreyScale)
			ZBuf.set(x, y, Color(newGreyScale));
	}

	
	void Renderer::UpdateFragColor(float xpos, float ypos, const Face& f, const Face& fOriginal, EQX_OUT Image& image, const Image& ZBuffer) const
	{
		// If camera disabled, only render white pixels
		if (!this->cameraEnabled)
		{
			image.set(xpos, ypos, Color::White);
			return;
		}

		/*  Full Pixel Processing  */
		int curGreyScale = ZBuffer.get(xpos, ypos).r;
		Color pixelColor = Color(0);
		Color texColor = Color(200); // TODO Texture Reading

		if (this->renderAAConfig == RenderAAConfig::ANTIALIAS_OFF)
		{
			if (!IsPointInTriangleZ(Vec2(xpos, ypos), f))
				return;

			// Set z to -Z_MAX if point is outside the triangle
			float zpos = f.ZAtXYFace(Vec2(xpos, ypos));
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseTransform * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.l.normal +
				baryCoord[1] * fOriginal.m.normal + baryCoord[2] * fOriginal.r.normal;

			float newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;
			if (newGreyScale > 255 || newGreyScale < 0)
				return;

			if (newGreyScale < curGreyScale + 1)
			{
				if (this->renderLightConfig == RenderLightConfig::PHONG)
				{
					for (const auto& l : this->lights)
					{
						if (l.lightType == LightType::Point)
						{
							Color resultColor = PhongLighting(originalPos, fragNormal, texColor, l);
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
		else if (this->renderAAConfig == RenderAAConfig::MSAA)
		{
			// Obtains z value presuming that the point is in the triangle
			float zpos = f.ZAtXYPlane(Vec2(xpos, ypos));
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseTransform * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.l.normal +
			baryCoord[1] * fOriginal.m.normal + baryCoord[2] * fOriginal.r.normal;

			Color originalColor = image.get(xpos, ypos);
			Color resultColor = Color(0);
			Face face(f);

			/*  Shading  */
			if (this->renderLightConfig == RenderLightConfig::PHONG)
				for (const auto& l : this->lights)
					if (l.lightType == LightType::Point)
						resultColor += PhongLighting(originalPos, fragNormal, texColor, l);

			/*  MSAA  */
			int samplerCnt = MSAAMult * MSAAMult;
			int validSamplerCnt = 0;
			float step = 1.f / (MSAAMult + 1);
			for (int i = 1; i <= MSAAMult; ++i)
			{
				for (int j = 1; j <= MSAAMult; ++j)
				{
					Vec2 curPos = Vec2(xpos - 0.5f + float(i) * step, ypos - 0.5f + float(j) * step);

					/*  Occlusion Test  */
					float z = face.ZAtXYFace(curPos.x, curPos.y);
					float newGreyScale = (z == 1) ? 0 : 128 - 127.f * z;
					if (newGreyScale >= curGreyScale + 2)
						continue;

					/*  Coverage Test  */
					if (IsPointInTriangleZ(curPos, face))
						++validSamplerCnt;
				}
			}
			resultColor = resultColor * validSamplerCnt / samplerCnt;
			resultColor = resultColor + originalColor;
			image.set(xpos, ypos, resultColor);
		}
	}

	Color Renderer::PhongLighting(Vec3 originalPos, Vec3 fragNormal, Color texColor, const Light& l) const
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
		return resultColor;
	}

}
