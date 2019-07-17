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
		mFilePath("")
	{
	}

	Texture::~Texture()
	{
	}

	int Texture::GetMaximumSize()
	{
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

	void Texture::Bind(const Texture& texture)
	{
	}

	void Texture::ActivateTextureUnit(unsigned textureUnit, const Texture& bindTexture)
	{
	}
}
