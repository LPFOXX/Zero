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

		inline static void Clear(unsigned bufferBits)
		{
			RenderCommand::sRendererAPI->clear(bufferBits);
		}

		inline static void SetViewportSize(unsigned width, unsigned height)
		{
			RenderCommand::sRendererAPI->setViewportSize(width, height);
		}

		inline static void EnableDepthTest(bool depthTestState)
		{
			RenderCommand::sRendererAPI->setDepthTestState(depthTestState);
		}

		inline static void EnableFaceCulling(bool enabled, RendererAPI::CullFace facesToCull = RendererAPI::CullFace::Back)
		{
			RenderCommand::sRendererAPI->setCullingFacesState(enabled, facesToCull);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			RenderCommand::sRendererAPI->drawIndexed(vertexArray);
		}

	private:
		static std::unique_ptr<RendererAPI> sRendererAPI;
	};
}
