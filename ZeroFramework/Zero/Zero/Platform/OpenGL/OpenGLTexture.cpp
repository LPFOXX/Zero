#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include <glad/glad.h>

#include "../../ImageReader.h"
#include "OpenGLTexture.h"
#include "../../Core/Profiller.h"

namespace zr
{
	unsigned OpenGLTexture2D::sCurrentlyBoundTexture = 0U;

	OpenGLTexture2D::OpenGLTexture2D() :
		Texture2D(),
		mTextureId(0U)
	{
		ZR_PROFILER_FUNCTION();

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, Texture2D::Type textureType) :
		Texture2D(),
		mTextureId(0U)
	{
		ZR_PROFILER_FUNCTION();

		if (!loadFromFile(filePath, true, textureType)) {
			ZR_CORE_ERROR("[OpenGLTexture] Can't load texture file: " + filePath);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ZR_PROFILER_FUNCTION();

		if (mTextureId != 0U) {
			GL_ERR_CHECK(glDeleteTextures(1, &mTextureId));
			mTextureId = 0U;
		}
	}

	int OpenGLTexture2D::GetMaximumSize()
	{
		int size;
		GL_ERR_CHECK(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));
		return size;
	}

	void OpenGLTexture2D::ActivateTextureSlot(unsigned textureSlot, unsigned textureHandle)
	{
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0 + textureSlot));
		if (OpenGLTexture2D::sCurrentlyBoundTexture != textureHandle) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, textureHandle));
			OpenGLTexture2D::sCurrentlyBoundTexture = textureHandle;
		}
	}

	void OpenGLTexture2D::bind() const
	{
		ZR_PROFILER_FUNCTION();

		if (OpenGLTexture2D::sCurrentlyBoundTexture != mTextureId) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			OpenGLTexture2D::sCurrentlyBoundTexture = mTextureId;
		}
	}

	void OpenGLTexture2D::unbind() const
	{
		ZR_PROFILER_FUNCTION();

		if (OpenGLTexture2D::sCurrentlyBoundTexture != 0U) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, 0U));
			OpenGLTexture2D::sCurrentlyBoundTexture = 0U;
		}
	}

	bool OpenGLTexture2D::loadFromFile(const std::string& filePath, bool flipVertically, Texture2D::Type textureType)
	{
		ZR_PROFILER_FUNCTION();

		int width, height, nrChannels;
		// Get image data
		unsigned char* data = ImageReader::LoadDataFromFile(filePath, width, height, nrChannels, flipVertically);

		bool returnResult = false;
		if (data && loadFromMemory(width, height, data, nrChannels)) {
			mFilePath = filePath;
			mTextureType = textureType;
			returnResult = true;
		}

		ImageReader::CleanData(data);
		return returnResult;
	}

	bool OpenGLTexture2D::loadFromMemory(unsigned width, unsigned height, const unsigned char* data, unsigned channelCount)
	{
		if (data) {
			unsigned internalFormat = 0, dataFormat = 0;
			if (channelCount == 4U) {
				internalFormat = mIsSRGBCapable ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channelCount == 3U) {
				internalFormat = mIsSRGBCapable ? GL_SRGB8 : GL_RGB8;
				dataFormat = GL_RGB;
			}

			if (!(internalFormat || dataFormat)) return false;

			GL_ERR_CHECK(glGenTextures(1, &mTextureId));
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			
			OpenGLTexture2D::TextureSaver saver;

			GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data));

			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));

			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

			// Update texture width and height attributes
			mSize.x = (float)width;
			mSize.y = (float)height;

			return true;
		}

		return false;
	}

	bool OpenGLTexture2D::loadFromImage(const Image& image, Texture2D::Type textureType)
	{
		ZR_PROFILER_FUNCTION();

		if (loadFromMemory(image.getWidth(), image.getHeight(), image.getData())) {
			mTextureType = textureType;
			mFilePath = image.getPath();
			return true;
		}
		return false;
	}

	void OpenGLTexture2D::bindOnTextureSlot(unsigned textureSlot)
	{
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0 + textureSlot));
		if (OpenGLTexture2D::sCurrentlyBoundTexture != mTextureId) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			OpenGLTexture2D::sCurrentlyBoundTexture = mTextureId;
		}
	}

	bool OpenGLTexture2D::update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos, unsigned yPos)
	{
		ZR_PROFILER_FUNCTION();

		if (mTextureId == 0U) {
			return false;
		}

		OpenGLTexture2D::TextureSaver saver;
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	bool OpenGLTexture2D::update(const Image& data, unsigned xPos, unsigned yPos)
	{
		ZR_PROFILER_FUNCTION();

		if (mTextureId == 0) {
			return false;
		}

		unsigned width = data.getWidth();
		unsigned height = data.getHeight();

		OpenGLTexture2D::TextureSaver saver;
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.getData()));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	bool OpenGLTexture2D::update(const std::vector<unsigned char>& data, unsigned width, unsigned height, unsigned xPos, unsigned yPos)
	{
		ZR_PROFILER_FUNCTION();

		if (mTextureId == 0) {
			return false;
		}

		OpenGLTexture2D::TextureSaver saver;
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	void OpenGLTexture2D::setSmooth(bool smoothFilterEnabled)
	{
		if (mIsSmooth != smoothFilterEnabled) {
			mIsSmooth = smoothFilterEnabled;

			if (mTextureId != 0U) {
				OpenGLTexture2D::TextureSaver saver;

				GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);

				if (mHasMipmaps) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
				}
				else {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
				}
			}
		}
	}

	void OpenGLTexture2D::setRepeat(bool repeatEnabled)
	{
		if (mIsRepeated != repeatEnabled) {
			mIsRepeated = repeatEnabled;

			if (mTextureId != 0U) {
				OpenGLTexture2D::TextureSaver saver;

				GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			}
		}
	}

	bool OpenGLTexture2D::generateMipMaps()
	{
		if (mTextureId == 0U)
			return false;

		OpenGLTexture2D::TextureSaver saver;

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR));

		mHasMipmaps = true;

		return true;
	}

	void OpenGLTexture2D::invalidateMipmaps()
	{
		if (!mHasMipmaps)
			return;

		OpenGLTexture2D::TextureSaver saver;

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));

		mHasMipmaps = false;
	}

	bool OpenGLTexture2D::loadToImage(Image& image) const
	{
		if (mTextureId) {
			int texWidth = 0, texHeight = 0;

			OpenGLTexture2D::TextureSaver saver;

			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth));
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight));

			if (texWidth != 0 && texHeight != 0) {
				std::vector<unsigned char> pixels(texWidth * texHeight * 4);
				GL_ERR_CHECK(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
				return image.create(texWidth, texHeight, pixels);;
			}
		}

		return false;
	}

	unsigned OpenGLTexture2D::getHandle() const
	{
		return mTextureId;
	}

	OpenGLTexture2D::TextureSaver::~TextureSaver()
	{
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mLastBoundTexture));
	}
}
