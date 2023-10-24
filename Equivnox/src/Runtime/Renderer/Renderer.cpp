#include "eqxpch.h"

#include "Renderer.h"

// unnecessary to explicitly write out conversion
#pragma warning(push)
#pragma warning(disable:4244)

namespace EQX
{
	Renderer::Renderer() : Renderer(400, 400) {  }

	Renderer::Renderer(XUInt width, XUInt height) :
		curScene(nullptr),
		cameraEnabled(true),
		hardShadow(true),
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
		assetManager(AssetManager::_init()),

		/*  Temporary Data Storage  */
		ssTransform(Mat4::Identity),
		perspTransform(Mat4::Identity),
		transform(Mat4::Identity),
		inverseTransform(Mat4::Identity),

		/*  Buffers and Outputs  */
		outputImage(width, height),
		ZBuffer(width, height),
		MSAAMask(width, height, (XChar)(MSAAMult* MSAAMult)),
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

	Renderer& Renderer::Init(XUInt width, XUInt height)
	{
#ifdef EQX_PRINT_STATUS
		EQX::Print("Welcome to Equivnox!");
#endif
		static Renderer _renderer (width, height);
		return _renderer;
	}

	void Renderer::BindScene(SceneInfo scene)
	{
#ifdef EQX_PRINT_STATUS
		Print("Loading Scene..");
#endif
		this->curScene = &_scene(scene);
	}

	void Renderer::UnbindScene()
	{
		this->curScene = nullptr;
	}

	void Renderer::SetMSAAMult(XUInt scale)
	{
		if (scale == 0 || scale == 1)
			MSAAMult = 2;
		else if (scale > 8)
			MSAAMult = 8;
		else
			MSAAMult = scale;

		this->MSAAMask.SetAll(MSAAMult * MSAAMult);
	}

	void Renderer::AddLight(Light l)
	{
		this->lightZMaps.emplace_back(this->width, this->height);
		this->lights.push_back(l);
	}

	SceneInfo Renderer::CreateEmptyScene(XString sceneName)
	{
		return assetManager._createEmptyScene(sceneName);
	}

	EntityInfo Renderer::CreateEmptyEntityUnderScene(SceneInfo scene, XString entityName)
	{
		return assetManager._createEmptyEntityUnderScene(scene, entityName);
	}

	EntityInfo Renderer::DuplicateEntity(SceneInfo curScene, XString from, XString to)
	{
		return assetManager._duplicateEntity(curScene, from, to);
	}

	EntityInfo Renderer::DuplicateEntityWithTransform(SceneInfo curScene, XString from, XString to, const MeshTransform& trans)
	{
		return assetManager._duplicateEntityWithTransform(curScene, from, to, trans);
	}

	void Renderer::Render()
	{
		/*  Check Validity  */
		if (!this->Validify())
		{
#ifdef EQX_DEBUG
			Print("Quitting...");
#endif
			return;
		}

		/*  Trigger Render Process  */
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
		for (XUInt x = 0; x < this->width; ++x)
			for (XUInt y = 0; y < this->height; ++y)
				outputImage.Set(x, y, Color::Black);

		/*  Setup Transformation Matrix  */
		Mat4 ssMat = MakeScreenSpace(this->width, this->height);
		Mat4 perspMat = perspMatFromCamera(this->camera);
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
			const Mesh& curMesh = curScene->FindEntityWithUID(entConfig->GetBoundUID()).mesh;

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
		constexpr XFloat reserveScale = 1.5;	// reserve scale for clipping meshes
		Scene& sceneToRender = this->assetManager._scene(Scene::s_rendererSceneName);
		SceneInfo sceneToRenderInfo = SceneInfo(sceneToRender.GetName(), sceneToRender.GetSceneID());

		// TODO Release the memory after reading infos on meshes

		/*  Frustum Clipping & Generating ZBuf for Shadow and Result */

		// Background of ZBuf should be white
		for (XUInt x = 0; x < this->width; ++x)
			for (XUInt y = 0; y < this->height; ++y)
				ZBuffer.Set(x, y, 255.0f);

		for (auto entConfig = curScene->Renderables().begin();
			entConfig != curScene->Renderables().cend(); ++entConfig)
		{
			if (entConfig->GetName() == EntityConfig::s_defaultEntityName)
				continue;

			const Mesh& curMesh = curScene->FindEntityWithUID(entConfig->GetBoundUID()).mesh;
			EntityInfo targetConfig = assetManager._createEmptyEntityUnderScene(sceneToRenderInfo, entConfig->GetName());
			Mesh& targetMesh = sceneToRender.FindEntityWithUID(targetConfig.entityID).mesh;
			std::vector<Vertex> vertices = {};
			for (auto iter = curMesh.vertices.begin(); iter != curMesh.vertices.cend(); ++iter)
				vertices.push_back(*iter);
			targetMesh.faceIndices.reserve(reserveScale * curMesh.faceIndices.size() * 3 * sizeof(std::array<XUInt, 3>));
			targetMesh.vertices.reserve(reserveScale * curMesh.vertices.size() * 3 * sizeof(Vertex));
			for (auto& vert : vertices)
				vert.MeshTransform(entConfig->GetTransform());

			// TODO Add Hard Shadows Here
			/*  Frustum Clipping & ZBuf for Hard Shadow  */


			/*  Frustum Clipping & ZBuf for Result  */
			for (auto& indexVec : curMesh.faceIndices)
			{
				Face f(vertices[indexVec[0]], vertices[indexVec[1]], vertices[indexVec[2]]);
				f.MatTransform(this->perspTransform);
				FrustumClipping(f, targetMesh);
			}
		}

		// TODO Compare and select
		/*if (cameraEnabled)
			for (auto entConfig = curScene->Renderables().begin();
				entConfig != curScene->Renderables().cend(); ++entConfig)
				RenderZBuf(entConfig, sceneToRender);*/


		/*  Z-Buffer Rendering  */

		if (cameraEnabled)
		{
			for (auto entConfig = sceneToRender.Renderables().begin();
				entConfig != sceneToRender.Renderables().cend(); ++entConfig)
			{
				if (entConfig->GetName() == EntityConfig::s_defaultEntityName)
					continue;

				const Mesh& curMesh = sceneToRender.FindEntityWithUID(entConfig->GetBoundUID()).mesh;
				for (auto& indexVec : curMesh.faceIndices)
				{
					Face f(curMesh.vertices[indexVec[0]], curMesh.vertices[indexVec[1]], curMesh.vertices[indexVec[2]]);
					f.MatTransform(this->ssTransform);
					RenderFaceZBuf(f);
				}
			}
		}

		/*  Rendering Only Z-Buffer  */
		if (this->renderPass == RenderPass::ZBUFFER_ONLY)
		{
			outputImage = ZBuffer;
			return;
		}

		/*  ZBuffer Prep for Shadow Rendering  */

		/*  Rendering Full Image  */
		for (auto entConfig = sceneToRender.Renderables().begin();
			entConfig != sceneToRender.Renderables().cend(); ++entConfig)
		{
			if (entConfig->GetName() == EntityConfig::s_defaultEntityName)
				continue;

			Mesh& curMesh = sceneToRender.FindEntityWithUID(entConfig->GetBoundUID()).mesh;
			for (auto& indexVec : curMesh.faceIndices)
			{
				Face fTransformed(curMesh.vertices[indexVec[0]], curMesh.vertices[indexVec[1]], curMesh.vertices[indexVec[2]]);
				/*  Back-Surface Culling  */
				if (Dot(fTransformed[0].normal, this->camera.lookAt) > 0 &&
					Dot(fTransformed[1].normal, this->camera.lookAt) > 0 &&
					Dot(fTransformed[2].normal, this->camera.lookAt) > 0)
					continue;

				/*  Render Each Fragment  */
				fTransformed.MatTransform(ssTransform);
				Face fOriginal(fTransformed);
				fOriginal.MatTransform(inverseTransform);
				RenderFaceSingle(fOriginal, fTransformed);
			}
		}
	}

	void Renderer::RenderLineRaw(const LineSeg& l)
	{
		LineSeg perspL(l.start, l.end);

		XUInt sx = static_cast<XUInt>(perspL.start.pos.x);
		XUInt sy = static_cast<XUInt>(perspL.start.pos.y);
		XUInt ex = static_cast<XUInt>(perspL.end.pos.x);
		XUInt ey = static_cast<XUInt>(perspL.end.pos.y);

		XBool transpose = false;
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

		for (XUInt x = sx; x != ex + 1; x += 1)
		{
			if (transpose)
			{
				XUInt y = static_cast<XUInt>(std::roundf(1 / perspL.k * (x - sx)) + sy);
				outputImage.Set(y, x, Color::White);
			}
			else
			{
				XUInt y = static_cast<XUInt>(std::roundf(perspL.k * (x - sx)) + sy);
				outputImage.Set(x, y, Color::White);
			}
		}
	}
	 
	void Renderer::RenderLineSmooth(const LineSeg& l)
	{
		LineSeg perspL(l.start, l.end);

		XUInt sx = static_cast<XUInt>(perspL.start.pos.x);
		XUInt sy = static_cast<XUInt>(perspL.start.pos.y);
		XUInt ex = static_cast<XUInt>(perspL.end.pos.x);
		XUInt ey = static_cast<XUInt>(perspL.end.pos.y);

		XUInt xPace = 1;
		XUInt yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
			for (XUInt x = sx; x != ex + xPace; x += xPace)
				outputImage.Set(x, sy, blendColor(Color::White, outputImage.Get(x, sy), 1.0));
		else if (abs(perspL.k) > SLOPE_MAX)
			for (XUInt y = sy; y != ey + yPace; y += yPace)
				outputImage.Set(sx, y, blendColor(Color::White, outputImage.Get(sx, y), 1.0));
		else
		{
			for (XUInt x = sx; x != ex; x += xPace)
			{
				for (XUInt y = static_cast<XUInt>(sy + perspL.k * (x - sx - 1)) - 2 * yPace;
					y != static_cast<XUInt>(sy + perspL.k * (x - sx + 2) + 2 * yPace);
					y += yPace)
					// only traverse pixels that will possibly be rendered
				{
					if (x >= this->width || y >= this->height)
						continue;
					Vector2 center(x + xPace / 2.0f, y + yPace / 2.0f);
					XFloat coeff = PixelAmp(perspL, center);
					outputImage.Set(x, y, blendColor(Color::White, outputImage.Get(x, y), coeff));
				}
			}
		}
	}

	void Renderer::RenderZBuf(std::vector<EntityConfig>::iterator entConfig, Scene& sceneToRender)
	{
		// TODO modify this to encapsulate the ZBuf rendering process in RenderFaces()
		// The following version does not work as Z-testing should be conducted w.r.t. faces
	
		//if (entConfig->GetName() == EntityConfig::s_defaultEntityName)
		//	return;

		//const Mesh& curMesh = sceneToRender.FindEntityWithUID(entConfig->GetBoundUID()).mesh;
		//for (auto& indexVec : curMesh.faceIndices)
		//{
		//	Face f(curMesh.vertices[indexVec[0]], curMesh.vertices[indexVec[1]], curMesh.vertices[indexVec[2]]);
		//	f.MatTransform(this->ssTransform);
		//	RenderFaceZBuf(f);
		//}
	}
	
	void Renderer::RenderFaceSingle(const Face& fOriginal, const Face& fTrans)
	{
		XFloat xpos, ypos;
		if (std::abs(fTrans.SlopeLM()) > SLOPE_MAX)
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.M().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() + 1;  
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else if (std::abs(fTrans.SlopeMR()) > SLOPE_MAX)
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else if (fTrans.SlopeLM() > fTrans.SlopeLR())
		// GetM() lies above the line segment connecting l and GetR()
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos < floor(fTrans.M().pos.x); ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() + 1; 
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
			for (xpos = std::floor(fTrans.M().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.M().pos.y + (xpos - fTrans.M().pos.x + sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() + 1; 
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
		else
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos < floor(fTrans.M().pos.x); ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x - sgn(fTrans.SlopeLM())) * fTrans.SlopeLM() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
			for (xpos = std::floor(fTrans.M().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.M().pos.y + (xpos - fTrans.M().pos.x - sgn(fTrans.SlopeMR())) * fTrans.SlopeMR() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x + sgn(fTrans.SlopeLR())) * fTrans.SlopeLR() + 1;
					++ypos)
					UpdateFragColor(xpos, ypos, fTrans, fOriginal);
		}
	}

	void Renderer::RenderFaceZBuf(const Face& fTrans)
	{
		XFloat xpos, ypos;
		if (std::abs(fTrans.SlopeLM()) > SLOPE_MAX)
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.M().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeMR() + 1; ++ypos)
					UpdateZBufColor(xpos, ypos, fTrans);
		}
		else if (fTrans.SlopeLM() > fTrans.SlopeLR())
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos <= fTrans.M().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLM() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
			for (xpos = std::floor(fTrans.M().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() - 1);
					ypos <= fTrans.M().pos.y + (xpos - fTrans.M().pos.x) * fTrans.SlopeMR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
		}
		else
		{
			for (xpos = std::floor(fTrans.L().pos.x); xpos <= fTrans.M().pos.x; ++xpos)
			{
				for (ypos = std::floor(fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLM() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
			}
			for (xpos = std::floor(fTrans.M().pos.x); xpos <= fTrans.R().pos.x; ++xpos)
				for (ypos = std::floor(fTrans.M().pos.y + (xpos - fTrans.M().pos.x) * fTrans.SlopeMR() - 1);
					ypos <= fTrans.L().pos.y + (xpos - fTrans.L().pos.x) * fTrans.SlopeLR() + 1; ++ypos)
					if (ZBuffer.IsPointOnCanvas(xpos, ypos))
						UpdateZBufColor(xpos, ypos, fTrans);
		}
	}

	XBool Renderer::Validify()
	{
		if (this->curScene == nullptr)
		{
			Print("[ERROR] Cannot render if no scene is binded!");
			return false;
		}
		else if (this->curScene->GetName() == Scene::s_defaultSceneName)
		{
			Print("[ERROR] Please select a valid scene to render!");
			return false;
		}
		else
		{
			XBool existNonEmpty = false;
			for (auto iter = this->curScene->Renderables().begin();
				iter != this->curScene->Renderables().cend(); ++iter)
			{
				if (iter->GetName() != EntityConfig::s_defaultEntityName)
				{
					Entity& ent = curScene->FindEntityWithUID(iter->GetBoundUID());
					if (!ent.mesh.Empty())
						existNonEmpty = true;
				}
			}
			if (!existNonEmpty)
			{
				Print("[WARNING] All Entities are empty. Nothing to render.");
				return false;
			}
		}
		
		return true;
	}

	void Renderer::ValidateConfig()
	{
		if (!this->cameraEnabled)
			this->renderPass = RenderPass::FULL;
	}

	Mat4 Renderer::perspMatFromCamera(Camera& c)
	{
		Mat4 perspMat;
		if (c.upDir == Vec4::Zero)
			perspMat = MakePersp(c.width, c.height) * MakeView(c.pos, c.lookAt);
		else
			perspMat = MakePersp(c.width, c.height) * MakeView(c.pos, c.lookAt, c.upDir);
		return perspMat;
	}

	void Renderer::UpdateZBufColor(XFloat x, XFloat y, const Face& f)
	{
		XFloat curGreyScale = ZBuffer.Get(x, y);

		XFloat zpos = f.ZAtXYFace(Vec2(x, y));
		XFloat newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;

		if (newGreyScale > 255 || newGreyScale < 0)
			return;

		if (newGreyScale < curGreyScale)
			ZBuffer.Set(x, y, newGreyScale);
	}

	
	void Renderer::UpdateFragColor(XFloat xpos, XFloat ypos, const Face& f, const Face& fOriginal)
	{
		// If camera disabled, only render white pixels
		if (!this->cameraEnabled)
		{
			outputImage.Set(xpos, ypos, Color::White);
			return;
		}

		/*  Full Pixel Processing  */
		XFloat curGreyScale = ZBuffer.Get(xpos, ypos);
		Color pixelColor = Color(0);
		Color texColor = Color(200); 

		if (this->renderAAConfig == RenderAAConfig::ANTIALIAS_OFF)
		{
			if (!IsPointInTriangle(Vec2(xpos, ypos), f))
				return;

			// Set z to -Z_MAX if point is outside the triangle
			XFloat zpos = f.ZAtXYFace(Vec2(xpos, ypos));
			Vec3 curPos = Vec3(xpos, ypos, zpos);
			Vec3 originalPos = inverseTransform * (curPos.ToVec4());
			Vec3 baryCoord = fOriginal.baryCoord(originalPos.x, originalPos.y);
			Vec3 fragNormal = baryCoord[0] * fOriginal.L().normal +
				baryCoord[1] * fOriginal.M().normal + baryCoord[2] * fOriginal.R().normal;

			XFloat newGreyScale = (zpos == 1) ? 0 : 128 - 127.f * zpos;
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
			XFloat zpos = f.ZAtXYPlane(Vec2(xpos, ypos));
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
			XUInt samplerCnt = MSAAMult * MSAAMult;
			XUInt validSamplerCnt = 0;
			XFloat step = 1.f / (MSAAMult + 1);

			for (XUInt i = 1; i <= MSAAMult; ++i)
			{
				for (XUInt j = 1; j <= MSAAMult; ++j)
				{
					Vec2 curPos = Vec2(xpos - 0.5f + i * step, ypos - 0.5f + j * step);

					/*  Occlusion Test  */
					XFloat z = face.ZAtXYFace(curPos.x, curPos.y);
					XFloat newGreyScale = (z == 1) ? 0 : 128 - 127.f * z;
					if (newGreyScale >= curGreyScale + 0.04)
						continue;

					/*  Coverage Test  */
					if (IsPointInTriangle(curPos, face))
						++validSamplerCnt;
				}
			}

			XUInt vacantSamplerCnt = XUInt(MSAAMask.Get(xpos, ypos));
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
		XFloat distance = (l.pos - originalPos).Norm();
		Vec3 lightDir = (l.pos - originalPos) / distance;
		Vec3 viewDir = (this->camera.pos.ToVec3() - originalPos).Normalize();
		Vector3 halfDir = (lightDir + viewDir).Normalize();
		Color ambient = 0.1f * texColor;
		Color diffuse = (LitColor(l.lightColor, texColor) * l.intensity) / (distance * distance) *
			std::max(0.f, Dot(fragNormal, lightDir));
		Color specular = l.lightColor * l.intensity / (distance * distance) *
			pow(std::max(0.f, Dot(fragNormal, halfDir)), 8);
		Color resultColor = ambient + diffuse + specular;
		return resultColor;
	}

	Scene& Renderer::_scene(XString sceneName)
	{
		return assetManager._scene(sceneName);
	}

	Scene& Renderer::_scene(SceneInfo sceneInfo)
	{
		return assetManager._scene(sceneInfo);
	}

	EntityConfig& Renderer::_configUnderScene(SceneInfo scene, XString entName)
	{
		return assetManager._configUnderScene(scene, entName);
	}

	Entity& Renderer::_entityUnderScene(SceneInfo sceneInfo, EntityID id)
	{
		return assetManager._entityUnderScene(sceneInfo, id);
	}

}
#pragma warning(pop)
