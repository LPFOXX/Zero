#pragma once

#include "../../Renderer/Texture.h"
#include "../../Image.h"
#include "../../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D();
		OpenGLTexture2D(const std::string& filePath, Texture2D::Type textureType);
		virtual ~OpenGLTexture2D();

		static int GetMaximumSize();
		static void ActivateTextureSlot(unsigned textureSlot, unsigned textureHandle);

		// Inherited via Texture2D
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual void bindOnTextureSlot(unsigned textureSlot) override;
		virtual bool update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual bool update(const Image& data, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual bool update(const std::vector<unsigned char>& data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual void setSmooth(bool smoothFilterEnabled) override;
		virtual void setRepeat(bool repeatEnabled) override;
		virtual bool generateMipMaps() override;
		virtual void invalidateMipmaps() override;
		virtual bool loadToImage(Image& image) const override;
		virtual unsigned getHandle() const override;
		virtual bool loadFromFile(const std::string& filePath, bool flipVertically = false, Texture2D::Type textureType = Texture2D::Type::None) override;
		virtual bool loadFromMemory(unsigned width, unsigned height, const unsigned char* data, unsigned channelCount = 4U) override;
		virtual bool loadFromImage(const Image& image, Texture2D::Type textureType = Texture2D::Type::None) override;

	private:
		class TextureSaver
		{
		public:
			TextureSaver() :
				mLastBoundTexture(OpenGLTexture2D::sCurrentlyBoundTexture)
			{
			}

			~TextureSaver();

		private:
			unsigned mLastBoundTexture;
		};

	private:
		static unsigned sCurrentlyBoundTexture;

	private:
		unsigned mTextureId;
	};
}
