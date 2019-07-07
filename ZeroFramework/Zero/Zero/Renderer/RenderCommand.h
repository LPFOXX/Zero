#pragma once

#include "RendererAPI.h"

namespace zr
{
	class RenderCommand
	{
	public:
		RenderCommand();
		virtual ~RenderCommand();

		inline static void SetClearColor(float r, float g, float b, float a)
		{
			RenderCommand::sRendererAPI->setClearColor(r, g, b, a);
		}

		inline static void Clear()
		{
			RenderCommand::sRendererAPI->clear();
		}

		inline static void SetViewportSize(unsigned width, unsigned height)
		{
			RenderCommand::sRendererAPI->setViewportSize(width, height);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			RenderCommand::sRendererAPI->drawIndexed(vertexArray);
		}

	private:
		static std::unique_ptr<RendererAPI> sRendererAPI;
	};
}
