#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "CubeMap.h"
#include "Text.h"

namespace zr
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void BeginScene(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Framebuffer>& framebuffer = nullptr);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		static void Submit(const std::shared_ptr<CubeMap>& cubeMap, bool ignoreTranslations = false);
		static void Submit(const std::shared_ptr<Text>& text);

		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}
	private:
		struct SceneData
		{
			Camera* Camera;
			//glm::mat4 ViewProjectionMatrix;
			std::shared_ptr<Framebuffer> Framebuffer;
		};

		static std::unique_ptr<SceneData> sSceneData;
	};
}
