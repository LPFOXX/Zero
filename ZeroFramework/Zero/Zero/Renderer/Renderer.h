#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace zr
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API getAPI()
		{
			return RendererAPI::GetAPI();
		}
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static std::unique_ptr<SceneData> sSceneData;
	};
}
