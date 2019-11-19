#include <zr_pch.h>

#include "../Renderer/Renderer.h"
#include "../Platform/OpenGL/OpenGLCubeMap.h"
#include "../Core/Core.h"

#include "CubeMap.h"

namespace zr
{
	CubeMap::CubeMap()
	{

	}

	CubeMap::~CubeMap()
	{

	}

	Ref<CubeMap> CubeMap::Create()
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
				return CreateRef<OpenGLCubeMap>();
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
