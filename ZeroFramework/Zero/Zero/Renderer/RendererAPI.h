#pragma once

#include "../Core/Core.h"

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

		enum StateBits
		{
			DepthBit = 1 << 0,
			BlendBit = 1 << 1
		};

		enum class CullFace
		{
			Front,
			Back
		};

		enum class DrawPrimitive
		{
			TriangleFan,
			TriangleStrip
		};

	public:
		RendererAPI();
		virtual ~RendererAPI();

		static Scope<RendererAPI> Create();
		virtual void init() = 0;
		virtual void setClearColor(float r, float g, float b, float a) = 0;
		virtual void clear(unsigned bufferBits) = 0;
		virtual void setViewportSize(unsigned width, unsigned height) = 0;
		virtual void setCullingFacesState(bool enabled, CullFace facesToCull) = 0;
		virtual bool getDepthTestState() = 0;
		virtual void setDepthTestState(bool depthEnabled) = 0;
		virtual void setBlendState(bool blendEnabled) = 0;
		virtual bool getBlendState() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void drawArrays(DrawPrimitive primitive, unsigned offset, unsigned count) = 0;
		
		inline static API GetAPI()
		{
			return RendererAPI::sAPI;
		}

	public:
		static API sAPI;
	};
}
