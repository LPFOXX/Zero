#include <zr_pch.h>

#include "RendererAPI.h"
#include "Zero/Platform/OpenGL/OpenGLRendererAPI.h"

namespace zr
{
	RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;

	RendererAPI::RendererAPI()
	{

	}

	RendererAPI::~RendererAPI()
	{

	}

	RendererAPI* RendererAPI::Create()
	{
		switch (RendererAPI::sAPI) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLRendererAPI;
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
