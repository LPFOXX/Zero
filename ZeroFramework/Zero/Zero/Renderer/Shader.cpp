#include <zr_pch.h>

#include "Shader.h"
#include "Zero/Renderer/Renderer.h"
#include "Zero/Platform/OpenGL/OpenGLShader.h"

namespace zr
{
	Shader::Shader()
	{

	}

	Ref<Shader> Shader::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>();
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

	Ref<Shader> Shader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(vertexShaderPath, fragmentShaderPath);
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

	Ref<Shader> Shader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
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
