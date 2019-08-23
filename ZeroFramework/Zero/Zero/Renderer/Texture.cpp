#include <zr_pch.h>

#include "RendererAPI.h"
#include "Texture.h"

#include "../Platform/OpenGL/OpenGLTexture.h"

namespace zr
{
	const std::string Texture2D::typeToString(Type textureType)
	{
		return std::string();
	}

	Texture2D::Texture2D() :
		mSize(0.f, 0.f),
		mTextureType(Type::None),
		mFilePath(""),
		mIsSRGBCapable(false),
		mIsSmooth(false),
		mIsRepeated(true),
		mHasMipmaps(false)
	{
	}

	Texture2D::~Texture2D()
	{
	}

	int Texture2D::GetMaximumSize()
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
				return OpenGLTexture2D::GetMaximumSize();
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

	Ref<Texture2D> Texture2D::Create()
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
				return std::make_shared<OpenGLTexture2D>();
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

	Ref<Texture2D> Texture2D::Create(const std::string& filePath, Type type)
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
				return std::make_shared<OpenGLTexture2D>(filePath, type);
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

	void Texture2D::Bind(const Texture2D& texture)
	{
	}

	void Texture2D::ActivateTextureUnit(unsigned textureUnit, const Texture2D& bindTexture)
	{
	}

	void Texture2D::ActivateTextureUnit(unsigned textureUnit, unsigned bindTexture)
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
				OpenGLTexture2D::ActivateTextureUnit(textureUnit, bindTexture);
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
