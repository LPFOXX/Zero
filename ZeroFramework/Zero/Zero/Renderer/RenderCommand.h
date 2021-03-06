#pragma once

#include "RendererAPI.h"

namespace zr
{
	class RenderCommand
	{
	public:
		RenderCommand();
		virtual ~RenderCommand();

		inline static void Init()
		{
			RenderCommand::sRendererAPI->init();
		}

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

		inline static void EnableDepthTest(bool depthTestState = true)
		{
			RenderCommand::sRendererAPI->setDepthTestState(depthTestState);
		}

		inline static void EnableFaceCulling(bool enabled, RendererAPI::CullFace facesToCull = RendererAPI::CullFace::Back)
		{
			RenderCommand::sRendererAPI->setCullingFacesState(enabled, facesToCull);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::DrawPrimitive drawPrimitive = RendererAPI::DrawPrimitive::Triangles)
		{
			RenderCommand::sRendererAPI->drawIndexed(vertexArray, drawPrimitive);
		}

		inline static void EnableBlend(bool blendEnabled)
		{
			RenderCommand::sRendererAPI->setBlendState(blendEnabled);
		}

		inline static bool GetDepthTestState()
		{
			return RenderCommand::sRendererAPI->getDepthTestState();
		}

		inline static int GetMaxElementsVertices()
		{
			return RenderCommand::sRendererAPI->getMaxElementsVertices();
		}

		inline static int GetMaxElementsIndices()
		{
			return RenderCommand::sRendererAPI->getMaxElementsIndices();
		}

		inline static void DrawArrays(RendererAPI::DrawPrimitive primitiveType, unsigned offset, unsigned count)
		{
			RenderCommand::sRendererAPI->drawArrays(primitiveType, offset, count);
		}

		inline static void MultiDrawIndexed(const Ref<VertexArray>& vertexArray, const IndexBuffer::Bounds& indexBounds, RendererAPI::DrawPrimitive drawPrimitive = RendererAPI::DrawPrimitive::Triangles)
		{
			RenderCommand::sRendererAPI->multiDrawIndexed(vertexArray, indexBounds, drawPrimitive);
		}

	private:
		static Scope<RendererAPI> sRendererAPI;
	};
}
