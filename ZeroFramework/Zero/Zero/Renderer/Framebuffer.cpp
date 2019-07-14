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

	Framebuffer* Framebuffer::Create(const FramebufferProperties& props)
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
				return new OpenGLFramebuffer(props);
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
