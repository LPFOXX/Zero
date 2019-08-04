#include <zr_pch.h>

#include "RendererAPI.h"
#include "Texture.h"

#include "../Platform/OpenGL/OpenGLTexture.h"

namespace zr
{
	const std::string Texture::typeToString(TextureType textureType)
	{
		return std::string();
	}

	Texture::Texture() :
		mSize(0U, 0U),
		mTextureType(TextureType::None),
		mFilePath(""),
		mIsSRGBCapable(false)
	{
	}

	Texture::~Texture()
	{
	}

	int Texture::GetMaximumSize()
	{
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:
			{
				return 0;
			}
			case RendererAPI::API::Direct3D:
			{
				return 0;
			}
			case RendererAPI::API::OpenGL:
			{
				return OpenGLTexture::GetMaximumSize();
			}
			case RendererAPI::API::Vulkan:
			{
				return 0;
			}
			default:
			break;
		}
		return 0;
	}

	Texture* Texture::Create()
	{
		switch (RendererAPI::GetAPI()) {
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
				return new OpenGLTexture;
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

	Texture* Texture::Create(const std::string& filePath, TextureType type)
	{
		switch (RendererAPI::GetAPI()) {
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
				return new OpenGLTexture(filePath, type);
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

	void Texture::Bind(const Texture& texture)
	{
	}

	void Texture::ActivateTextureUnit(unsigned textureUnit, const Texture& bindTexture)
	{
	}

	void Texture::ActivateTextureUnit(unsigned textureUnit, unsigned bindTexture)
	{
		switch (RendererAPI::GetAPI()) {
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
				OpenGLTexture::ActivateTextureUnit(textureUnit, bindTexture);
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
}
