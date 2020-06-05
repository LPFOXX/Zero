#include <zr_pch.h>

#include "Framebuffer.h"
#include "Renderer.h"
#include "Zero/Platform/OpenGL/OpenGLFramebuffer.h"

namespace zr
{
	Framebuffer::Framebuffer()
	{
	}

	Framebuffer::~Framebuffer()
	{
	}

	void Framebuffer::BindDefault()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				break;
			}
			case RendererAPI::API::Direct3D:
			{
				break;
			}
			case RendererAPI::API::OpenGL:
			{
				OpenGLFramebuffer::BindDefault();
				break;
			}
			case RendererAPI::API::Vulkan:
			{
				break;
			}
			default:
			break;
		}
	}

	unsigned Framebuffer::GetMaxSamples()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return 1;
			}
			case RendererAPI::API::Direct3D:
			{
				return 1;
			}
			case RendererAPI::API::OpenGL:
			{
				return OpenGLFramebuffer::GetMaxSamples();
			}
			case RendererAPI::API::Vulkan:
			{
				return 1;
			}
			default:
			break;
		}
		return 1;
	}

	std::pair<unsigned, unsigned> Framebuffer::GetMaxViewportSize()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return { 1280, 720 };
			}
			case RendererAPI::API::Direct3D:
			{
				return { 1280, 720 };
			}
			case RendererAPI::API::OpenGL:
			{
				return OpenGLFramebuffer::GetMaxViewportSize();
			}
			case RendererAPI::API::Vulkan:
			{
				return { 1280, 720 };
			}
			default:
			break;
		}
		return { 1280, 720 };
	}

	Ref<Framebuffer> Framebuffer::Create(const Properties& props)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::Direct3D:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLFramebuffer>(props);
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
