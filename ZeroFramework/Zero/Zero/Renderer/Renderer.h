#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "CubeMap.h"
#include "Text.h"
#include "Sprite.h"

namespace zr
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void Init();
		static void Shutdown();
		static void BeginScene(const Ref<Camera>& camera, const Ref<Framebuffer>& framebuffer = nullptr);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		static void Submit(const Ref<CubeMap>& cubeMap, bool ignoreTranslations = false);
		static void Submit(const Ref<Text>& text);
		static void Submit(const Ref<Sprite>& sprite);

		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}
	private:
		struct SceneData
		{
			Camera* Camera;
			std::shared_ptr<Framebuffer> Framebuffer;
		};

		static Scope<SceneData> sSceneData;
	};
}
