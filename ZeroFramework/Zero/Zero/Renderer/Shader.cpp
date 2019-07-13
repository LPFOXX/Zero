#include <zr_pch.h>

#include "Shader.h"
#include "Zero/Renderer/Renderer.h"
#include "Zero/Platform/OpenGL/OpenGLShader.h"

namespace zr
{
	Shader::Shader()
	{

	}

	Shader* Shader::create()
	{
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLShader;
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

	Shader* Shader::create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLShader(vertexShaderPath, fragmentShaderPath);
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

	Shader* Shader::create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
	{
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLShader(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
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

	Shader::~Shader()
	{
	}
}
