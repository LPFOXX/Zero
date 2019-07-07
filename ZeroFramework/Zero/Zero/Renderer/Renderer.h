#pragma once

#include "RenderCommand.h"

namespace zr
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API getAPI()
		{
			return RendererAPI::GetAPI();
		}
	};
}
