#pragma once

#include "Buffer.h"

namespace zr
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL,
			Direct3D,
			Vulkan
		};

		enum ClearBuffers
		{
			Color =				1 << 0,
			Depth =				1 << 1,
			Stencil =			1 << 2
		};

		enum class CullFace
		{
			Front,
			Back
		};

	public:
		RendererAPI();
		virtual ~RendererAPI();

		static RendererAPI* Create();
		virtual void setClearColor(float r, float g, float b, float a) = 0;
		virtual void clear(unsigned bufferBits) = 0;
		virtual void setViewportSize(unsigned width, unsigned height) = 0;
		virtual void setCullingFacesState(bool enabled, CullFace facesToCull) = 0;
		virtual bool getDepthTestState() = 0;
		virtual void setDepthTestState(bool state) = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		
		inline static API GetAPI()
		{
			return RendererAPI::sAPI;
		}

	public:
		static API sAPI;
	};
}