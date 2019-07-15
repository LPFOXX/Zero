#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "CubeMap.h"

namespace zr
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void BeginScene(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Framebuffer>& framebuffer = nullptr);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		static void Submit(const std::shared_ptr<CubeMap>& cubeMap);

		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			std::shared_ptr<Framebuffer> Framebuffer;
		};

		static std::unique_ptr<SceneData> sSceneData;
	};
}
