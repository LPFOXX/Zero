#pragma once

#include "Zero/Renderer/Buffer.h"

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

	public:
		RendererAPI();
		virtual ~RendererAPI();

		static RendererAPI* Create();
		virtual void setClearColor(float r, float g, float b, float a) = 0;
		virtual void clear() = 0;
		virtual void setViewportSize(unsigned width, unsigned height) = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		
		inline static API GetAPI()
		{
			return RendererAPI::sAPI;
		}

	public:
		static API sAPI;
	};
}