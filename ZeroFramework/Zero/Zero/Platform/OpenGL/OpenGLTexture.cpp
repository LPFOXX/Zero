#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include <glad/glad.h>
#include "OpenGLTexture.h"
#include "../../Image.h"

namespace zr
{
	OpenGLTexture::OpenGLTexture() :
		Texture(),
		mTextureId(0U)
	{

	}

	OpenGLTexture::OpenGLTexture(const std::string& filePath, TextureType type) :
		Texture()
	{
		mFilePath = filePath;
		mTextureType = type;

		if (!loadFromFile(filePath, true)) {
			throw std::runtime_error("Can't load texture: " + mFilePath);
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{

	}

	void OpenGLTexture::bind() const
	{
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
	}

	void OpenGLTexture::bindOnTextureUnit(unsigned textureUnit)
	{
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
	}

	bool OpenGLTexture::loadFromFile(const std::string& fileName, bool flipVertically)
	{
		GL_ERR_CHECK(glGenTextures(1, &mTextureId));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));

		int width, height, nrChannels;
		// Get image data
		unsigned char* data = ImageLoader::loadDataFromFile(fileName, &width, &height, &nrChannels, flipVertically);

		if (setTextureData(width, height, 4U, data)) {
			// Update texture width and height attributes
			mSize.x = width;
			mSize.y = height;
			mFilePath = fileName;
			ImageLoader::cleanData(data);
			return true;
		}
		else {
			ImageLoader::cleanData(data);
			return false;
		}

		return true;
	}

	bool OpenGLTexture::loadFromFile(const std::string& fileName, Texture::TextureType textureType, bool flipVertically)
	{
		mTextureType = textureType;
		return loadFromFile(fileName, flipVertically);
	}

	bool OpenGLTexture::loadFromImage(const Image& image)
	{
		int imageWidth = image.getWidth();
		int imageHeight = image.getHeight();

		if (imageWidth == 0 || imageHeight == 0) {
			return false;
		}

		GL_ERR_CHECK(glGenTextures(1, &mTextureId));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));

		const unsigned char* imageData = image.getData();

		if (setTextureData(imageWidth, imageHeight, image.getChannelCount(), imageData)) {
			mSize.x = imageWidth;
			mSize.y = imageHeight;
			return true;
		}
		else {
			GL_ERR_CHECK(glDeleteTextures(1, &mTextureId));
		}

		return false;
	}

	bool OpenGLTexture::update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos, unsigned yPos)
	{
		if (mTextureId == 0) {
			return false;
		}

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	bool OpenGLTexture::update(const Image& data, unsigned xPos, unsigned yPos)
	{
		if (mTextureId == 0) {
			return false;
		}

		unsigned width = data.getWidth();
		unsigned height = data.getHeight();

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.getData()));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	bool OpenGLTexture::update(const std::vector<unsigned char>& data, unsigned width, unsigned height, unsigned xPos, unsigned yPos)
	{
		if (mTextureId == 0) {
			return false;
		}

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());
		return true;
	}

	bool OpenGLTexture::loadToImage(Image& image) const
	{
		image = Image();

		if (mTextureId != 0U) {
			int texWidth = 0, texHeight = 0;

			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth));
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight));

			if (texWidth != 0 && texHeight != 0) {
				std::vector<unsigned char> pixels(static_cast<size_t>(texWidth * texHeight * 4));
				GL_ERR_CHECK(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
				return image.create(texWidth, texHeight, pixels);;
			}
		}

		return false;
	}

	unsigned OpenGLTexture::getHandle() const
	{
		return mTextureId;
	}

	bool OpenGLTexture::setTextureData(unsigned width, unsigned height, unsigned channelCount, const unsigned char* data)
	{
		if (data) {
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			GLenum format;
			if (channelCount == 4U) {
				format = GL_RGBA;
				/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			}
			else if (channelCount == 3U) {
				format = GL_RGB;
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			}
			else if (channelCount == 2U) {
				format = GL_RG;
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			}
			else if (channelCount == 1U) {
				format = GL_RED;
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			}

			GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			GL_ERR_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			GL_ERR_CHECK(glFlush());
			return true;
		}
		return false;
	}
}
