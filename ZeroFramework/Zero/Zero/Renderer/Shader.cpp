#include <zr_pch.h>

#include "Shader.h"
#include "Zero/Renderer/Renderer.h"
#include "Zero/Platform/OpenGL/OpenGLShader.h"

namespace zr
{
	Shader::Shader()
	{

	}

	Shader::~Shader()
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
				return CreateRef<OpenGLShader>();
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

	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLShader>(filePath);
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

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLShader>(name, vertexShaderPath, fragmentShaderPath);
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

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLShader>(name, vertexShaderPath, fragmentShaderPath, geometryShaderPath);
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

	ShaderLibrary::ShaderLibrary() :
		mShaders()
	{

	}

	ShaderLibrary::~ShaderLibrary()
	{

	}

	void ShaderLibrary::add(const std::string& shaderName, const Ref<Shader>& shader)
	{
		if (exists(shaderName)) return;
		mShaders[shaderName] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& shaderName, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		add(shaderName, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& shaderName)
	{
		if (exists(shaderName)) {
			return mShaders[shaderName];
		}
		return nullptr;
	}

	bool ShaderLibrary::exists(const std::string& shaderName) const
	{
		return mShaders.find(shaderName) != mShaders.end();
	}
}