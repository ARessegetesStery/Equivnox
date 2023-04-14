#include "eqxpch.h"

#include "Renderer.h"

namespace EQX
{
	Renderer::Renderer() : Renderer(400, 400) {  }

	Renderer::Renderer(unsigned int width, unsigned int height) :
		curScene(nullptr),
		cameraEnabled(true),
		renderFill(RenderFill::WIREFRAME),
		renderPass(RenderPass::FULL),
		renderAAConfig(RenderAAConfig::ANTIALIAS_OFF),
		renderLightConfig(ShadingMode::RASTERIZE),
		imageType(ImageType::TGA),
		outputPath("output"),
		width(width),
		height(height),
		wScale(1.0f),
		hScale(1.0f),
		MSAAMult(4),

		/*  Temporary Data Storage  */
		ssTransform(Mat4::Identity),
		perspTransform(Mat4::Identity),
		transform(Mat4::Identity),
		inverseTransform(Mat4::Identity),

		/*  Buffers and Outputs  */
		outputImage(width, height),
		ZBuffer(width, height),
		MSAAMask(width, height, (char)(MSAAMult* MSAAMult)),
		lightZMaps({})
	{  }

	Renderer& Renderer::Init()
	{
#ifdef EQX_PRINT_STATUS
		EQX::Print("Welcome to Equivnox!");
#endif
		static Renderer _renderer;
		return _renderer;
	}

	Renderer& Renderer::Init(unsigned int width, unsigned int height)
	{
#ifdef EQX_PRINT_STATUS
		EQX::Print("Welcome to Equivnox!");
#endif
		static Renderer _renderer (width, height);
		return _renderer;
	}

	void Renderer::BindScene(Scene* scene)
	{
#ifdef EQX_PRINT_STATUS
		Print("Loading Scene..");
#endif
		this->curScene = scene;
	}

	void Renderer::UnbindScene()
	{
		this->curScene = nullptr;
	}

	void Renderer::SetMSAAMult(unsigned int scale)
	{
		if (scale == 0 || scale == 1)
			MSAAMult = 2;
		else if (scale > 8)
			MSAAMult = 8;
		else
			MSAAMult = scale;

		this->MSAAMask.SetAll(MSAAMult * MSAAMult);
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
		if (this->renderLightConfig == ShadingMode::RASTERIZE)
			this->Rasterize();
		else if (this->renderLightConfig == ShadingMode::RAYTRACING)
			this->Raytrace();
	}

	void Renderer::Raytrace()
	{

	}

	void Renderer::Rasterize()
	{
#ifdef EQX_PRINT_STATUS
		Print("Rendering Faces [Mode: Rasterizer]..");
#endif

		/*  Initializing the background color to be black  */
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				outputImage.Set(x, y, Color::Black);

		/*  Setup Transformation Matrix  */
		Mat4 ssMat = MakeScreenSpace(this->width, this->height);
		Mat4 perspMat = Mat4::Identity;
		if (this->camera.upDir == Vec4::Zero)
			perspMat = MakePersp(camera.width, camera.height) * MakeView(camera.pos, camera.lookAt);
		else
			perspMat = MakePersp(camera.width, camera.height) * MakeView(camera.pos, camera.lookAt, camera.upDir);
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
			RenderLines();
			break;
		case RenderFill::FILL:
			RenderFaces();
			break;
		default:
			break;
		}

		if (this->wScale != 1.0f || this->hScale != 1.0f)
			outputImage.Rescale(wScale * this->width, hScale * this->height);

#ifdef EQX_PRINT_STATUS
		Print("Writing into output file..");
#endif
		outputImage.Write(this->imageType, this->outputPath);
	}

	void Renderer::RenderLines()
	{
		Mat4 PerspMat = Mat4::Identity;
		if (this->camera.upDir == Vec4::Zero || Dot(this->camera.upDir, this->camera.lookAt) != 0)
			PerspMat = MakeScreenSpace(this->width, this->height) *
				MakePersp(camera.width, camera.height, -camera.nearClip, -camera.farClip) *
				MakeView(camera.pos, camera.lookAt);
		else
			PerspMat = MakeScreenSpace(this->width, this->height) *
				MakePersp(camera.width, camera.height, -camera.nearClip, -camera.farClip) * 
				MakeView(camera.pos, camera.lookAt, camera.upDir);

		Mat4 Projection = Mat4::Identity;
		if (this->cameraEnabled)
			Projection = PerspMat;

		for (auto entConfig = curScene->Renderables().begin();
			entConfig != curScene->Renderables().cend(); ++entConfig)
		{
			const Mesh& curMesh = curScene->FindUID(entConfig->GetUID()).GetMesh();

			for (auto iter = curMesh.lineIndices.begin();
				iter != curMesh.lineIndices.cend(); ++iter)
			{
				LineSeg l(curMesh.vertices[(*iter)[0]], curMesh.vertices[(*iter)[1]]);
				l.Transform(entConfig->GetTransform());
				l.Transform(Projection);

				switch (this->renderAAConfig)
				{
				case RenderAAConfig::ANTIALIAS_OFF:
					RenderLineRaw(l);
					break;
				case RenderAAConfig::MSAA: case RenderAAConfig::SMOOTH:
					RenderLineSmooth(l);
					break;
				}
			}
			for (auto iter = curMesh.faceIndices.begin();
				iter != curMesh.faceIndices.cend(); ++iter)
			{
				std::array<LineSeg, 3> lines;
				lines[0] = LineSeg(curMesh.vertices[(*iter)[0]], curMesh.vertices[(*iter)[1]]);
				lines[1] = LineSeg(curMesh.vertices[(*iter)[0]], curMesh.vertices[(*iter)[2]]);
				lines[2] = LineSeg(curMesh.vertices[(*iter)[1]], curMesh.vertices[(*iter)[2]]);
				for (auto& l : lines)
				{
					l.Transform(entConfig->GetTransform());
					l.Transform(Projection);
				}

				switch (this->renderAAConfig)
				{
				case RenderAAConfig::ANTIALIAS_OFF:
					for (auto& l : lines)
						RenderLineRaw(l);
					break;
				case RenderAAConfig::MSAA: case RenderAAConfig::FXAA:
					for (auto& l : lines)
						RenderLineSmooth(l);
					break;
				}
			}
		}
	}

	void Renderer::RenderFaces()
	{
		constexpr double reserveScale = 1.5;	// reserve scale for clipping meshes
		std::vector<FMesh> clippedMesh; 

		/*  Frustum Clipping  */
		for (auto entConfig = curScene->Renderables().begin();
			entConfig != curScene->Renderables().cend(); ++entConfig)
		{
			const Mesh& curMesh = curScene->FindUID(entConfig->GetUID()).GetMesh();
			clippedMesh.emplace_back(FMesh());
			auto curFMesh = clippedMesh.end() - 1;
			curFMesh->faces.reserve(reserveScale * curMesh.faceIndices.size() * 3 * sizeof(Vertex));
			for (auto iter = curMesh.faceIndices.begin();
				iter != curMesh.faceIndices.cend(); ++iter)
			{
				std::array<Vertex, 3> vertices{ curMesh.vertices[(*iter)[0]],
					curMesh.vertices[(*iter)[1]] , curMesh.vertices[(*iter)[2]] };
				Face f(vertices);
				f.Transform(entConfig->GetTransform());
				f.Transform(this->perspTransform);
				FrustumClipping(f, *curFMesh);
			}
		}

		/*  Z-Buffer Rendering  */

		// Background of ZBuf should be white
		for (int x = 0; x < this->width; ++x)
			for (int y = 0; y < this->height; ++y)
				ZBuffer.Set(x, y, 255.0f);

		for (auto curMesh = clippedMesh.begin(); curMesh != clippedMesh.cend(); ++curMesh)
		{
			if (this->cameraEnabled)
			{
				for (const auto& mesh : clippedMesh)
				{
					for (const auto& f : mesh.faces)
					{
						Face fTransformed(f);
						fTransformed.Transform(this->ssTransform);
						RenderFaceZBuf(fTransformed);
					}
				}
			}
		}

		/*  Rendering Only Z-Buffer  */
		if (this->renderPass == RenderPass::ZBUFFER_ONLY)
		{
			outputImage = ZBuffer;
			return;
		}

		/*  Rendering Full Image  */
		for (auto& mesh : clippedMesh)
		{
			for (auto& fTransformed : mesh.faces)
			{
				/*  Back-Surface Culling  */
				if (Dot(fTransformed[0].normal, this->camera.lookAt) > 0 &&
					Dot(fTransformed[1].normal, this->camera.lookAt) > 0 &&
					Dot(fTransformed[2].normal, this->camera.lookAt) > 0)
					continue;

				/*  Render Each Fragment  */
				fTransformed.Transform(ssTransform);
				Face fOriginal(fTransformed);
				fOriginal.Transform(inverseTransform);
				RenderFaceSingle(fOriginal, fTransformed);
			}
		}
	}

	void Renderer::RenderLineRaw(const LineSeg& l)
	{
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
				outputImage.Set(y, x, Color::White);
			}
			else
			{
				int y = std::roundf(perspL.k * (x - sx)) + sy;
				outputImage.Set(x, y, Color::White);
			}
		}
	}
	 
	void Renderer::RenderLineSmooth(const LineSeg& l)
	{
		LineSeg perspL(l.start, l.end);

		int sx = static_cast<int>(perspL.start.pos.x);
		int sy = static_cast<int>(perspL.start.pos.y);
		int ex = static_cast<int>(perspL.end.pos.x);
		int ey = static_cast<int>(perspL.end.pos.y);

		int xPace = 1;
		int yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
			for (int x = sx; x != ex + xPace; x += xPace)
				outputImage.Set(x, sy, blendColor(Color::White, outputImage.Get(x, sy), 1.0));
		else if (abs(perspL.k) > SLOPE_MAX)
			for (int y = sy; y != ey + yPace; y += yPace)
				outputImage.Set(sx, y, blendColor(Color::White, outputImage.Get(sx, y), 1.0));
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
					outputImage.Set(x, y, blendColor(Color::White, outputImage.Get(x, y), coeff));
				}
			}
		}
	}
	
	void Renderer::RenderFaceSingle(const Face& fOriginal, const Face& fTrans)
	{
		int xpos, ypos;
		if (std::abs(fTrans.SlopeLM()) > SLOPE_MAX)
		{
			for (xpos = fTrans.L().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.M().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else if (std::abs(fTrans.SlopeMR()) > SLOPE_MAX)
		{
			for (xpos = fTrans.L().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else if (fTrans.SlopeLM() > fTrans.SlopeLR())
		// GetM() lies above the line segment connecting l and GetR()
		{
			for (xpos = fTrans.L().pos.x; xpos < floor(fTrans.M().pos.x); ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() + 1; 
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
			for (xpos = fTrans.M().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.M().pos.y + (xpos - fTrans.M().pos.x + sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() + 1; 
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else
		{
			for (xpos = fTrans.L().pos.x; xpos < floor(fTrans.M().pos.x); ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
			for (xpos = fTrans.M().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.M().pos.y + (xpos - fTrans.M().pos.x - sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
	}

	void Renderer::RenderFaceZBuf(const Face& fTrans)
	{
		int xpos, ypos;
		if (std::abs(fTrans.SlopeLM()) > SLOPE_MAX)
		{
			for (xpos = fTrans.L().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.M().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeMR() + 1; ++ypos)
					UpdateZBufColor(xpos, ypos, fTrans);
		}
		else if (fTrans.SlopeLM() > fTrans.SlopeLR())
		{
			for (xpos = fTrans.L().pos.x; xpos <= fTrans.M().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLM() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
			for (xpos = fTrans.M().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1;
					ypos <= fTrans.M().pos.y + (xpos - fTrans.M().pos.x) * fTrans.SlopeMR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
		}
		else
		{
			for (xpos = fTrans.L().pos.x; xpos <= fTrans.M().pos.x; ++xpos)
			{
				for (ypos = fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLM() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
			}
			for (xpos = fTrans.M().pos.x; xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = fTrans.M().pos.y + (xpos - fTrans.M().pos.x) * fTrans.SlopeMR() - 1;
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
		}
	}

	void Renderer::UpdateZBufColor(float x, float y, const Face& f)
	{
		float curGreyScale = ZBuffer.Get(x, y);

		float zpos = f.ZAtXYFace(Vec2(x, y));
		float newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;

		if (newGreyScale > 255 || newGreyScale < 0)
			return;

		if (newGreyScale < curGreyScale)
			ZBuffer.Set(x, y, newGreyScale);
	}

	
	void Renderer::UpdateFragColor(float xpos, float ypos, const Face& f, const Face& fOriginal)
	{
		// If camera disabled, only render white pixels
		if (!this->cameraEnabled)
		{
			outputImage.Set(xpos, ypos, Color::White);
			return;
		}

		/*  Full Pixel Processing  */
		float curGreyScale = ZBuffer.Get(xpos, ypos);
		Color pixelColor = Color(0);
		Color texColor = Color(200); // TODO Texture Reading

		if (this->renderAAConfig == RenderAAConfig::ANTIALIAS_OFF)
		{
			if (!IsPointInTriangle(Vec2(xpos, ypos), f))
				return;

			// Set z to -Z_MAX if point is outside the triangle
			float zpos = f.ZAtXYFace(Vec2(xpos, ypos));
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseTransform * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.L().normal +
				baryCoord[1] * fOriginal.M().normal + baryCoord[2] * fOriginal.R().normal;

			float newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;
			if (newGreyScale > 255 || newGreyScale < 0)
				return;

			if (newGreyScale <= curGreyScale)
			{
				for (const auto& l : this->lights)
				{
					if (l.lightType == LightType::Point)
					{
						Color resultColor = PhongLighting(originalPos, fragNormal, texColor, l);
						pixelColor = pixelColor + resultColor;
					}
				}
				outputImage.Set(xpos, ypos, pixelColor);
			}
		}
		else if (this->renderAAConfig == RenderAAConfig::MSAA)
		{
			// Obtains z value presuming that the point is in the triangle
			float zpos = f.ZAtXYPlane(Vec2(xpos, ypos));
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseTransform * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.L().normal +
				baryCoord[1] * fOriginal.M().normal + baryCoord[2] * fOriginal.R().normal;

			Color originalColor = outputImage.Get(xpos, ypos);
			Color resultColor = Color(0);
			Face face(f);

			/*  Shading  */
			if (this->renderLightConfig == ShadingMode::RASTERIZE)
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
					if (newGreyScale >= curGreyScale + 0.04)
						continue;

					/*  Coverage Test  */
					if (IsPointInTriangle(curPos, face))
						++validSamplerCnt;
				}
			}

			int vacantSamplerCnt = int(MSAAMask.Get(xpos, ypos));
			if (vacantSamplerCnt <= validSamplerCnt)
				validSamplerCnt = vacantSamplerCnt;
			MSAAMask.Set(xpos, ypos, vacantSamplerCnt - validSamplerCnt);

			resultColor = resultColor * validSamplerCnt / samplerCnt;
			resultColor = resultColor + originalColor;

			outputImage.Set(xpos, ypos, resultColor);
		}
	}

	Color Renderer::PhongLighting(Vec3 originalPos, Vec3 fragNormal, Color texColor, const Light& l)
	{
		float distance = (l.pos - originalPos).Norm();
		Vec3 lightDir = (l.pos - originalPos) / distance;
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
