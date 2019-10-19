#include <zr_pch.h>

#include "Buffer.h"
#include "Renderer.h"

#include "Zero/Platform/OpenGL/OpenGLBuffer.h"
#include "Zero/Platform/OpenGL/OpenGLVertexArray.h"

namespace zr
{
	VertexBuffer::VertexBuffer()
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned size, DrawMode drawMode)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(vertices, size, drawMode);
			}
			case RendererAPI::API::Direct3D:
			{
				return nullptr;
			}
			case RendererAPI::API::Vulkan:
			{
				return nullptr;
			}
			default:
			break;
		}
		return nullptr;
	}

	IndexBuffer::IndexBuffer()
	{
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	Ref<IndexBuffer> IndexBuffer::Create(unsigned* data, unsigned count, DrawMode drawMode)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(data, count, drawMode);
			}
			case RendererAPI::API::Direct3D:
			{
				return nullptr;
			}
			case RendererAPI::API::Vulkan:
			{
				return nullptr;
			}
			default:
			break;
		}
		return nullptr;
	}

	VertexArray::VertexArray()
	{
	}

	VertexArray::~VertexArray()
	{
	}

	Ref<VertexArray> VertexArray::Create(bool createAndBind)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>(createAndBind);
			}
			case RendererAPI::API::Direct3D:
			{
				return nullptr;
			}
			case RendererAPI::API::Vulkan:
			{
				return nullptr;
			}
			default:
			break;
		}
		return nullptr;
	}
}
