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

	Ref<Framebuffer> Framebuffer::Create(const FramebufferProperties& props)
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
