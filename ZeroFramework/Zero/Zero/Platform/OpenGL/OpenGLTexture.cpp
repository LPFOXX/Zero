#include <zr_pch.h>

#include "GL_ERR_CHECK.h"
#include <glad/glad.h>
#include "OpenGLTexture.h"
#include "../../Renderer/Image.h"
#include "../../Renderer/ImageReader.h"

namespace zr
{
	unsigned OpenGLTexture::sCurrentlyBoundTexture = 0U;

	OpenGLTexture::OpenGLTexture() :
		Texture(),
		mTextureId(0U),
		mIsSmooth(false),
		mIsRepeated(true),
		mHasMipmaps(false)
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
		if (mTextureId != 0U) {
			GL_ERR_CHECK(glDeleteTextures(1, &mTextureId););
			mTextureId = 0U;
		}
	}

	void OpenGLTexture::ActivateTextureUnit(unsigned textureUnit, unsigned bindTexture)
	{
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, bindTexture));
		OpenGLTexture::sCurrentlyBoundTexture = bindTexture;
	}

	int OpenGLTexture::GetMaximumSize()
	{
		int size;
		GL_ERR_CHECK(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));
		return size;
	}

	void OpenGLTexture::bind() const
	{
		if (OpenGLTexture::sCurrentlyBoundTexture != mTextureId) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
		}
	}

	void OpenGLTexture::bindOnTextureUnit(unsigned textureUnit)
	{
		GL_ERR_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
	}

	bool OpenGLTexture::loadFromFile(const std::string& fileName, bool flipVertically)
	{
		GL_ERR_CHECK(glGenTextures(1, &mTextureId));
		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;

		int width, height, nrChannels;
		// Get image data
		unsigned char* data = ImageReader::loadDataFromFile(fileName, width, height, nrChannels, flipVertically);

		if (data) {
			GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, mIsSRGBCapable ? GL_SRGB8_ALPHA8 : GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));

			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

			GL_ERR_CHECK(glFlush());

			// Update texture width and height attributes
			mSize.x = width;
			mSize.y = height;
			mFilePath = fileName;
			ImageReader::cleanData(data);

			return true;
		}
		else {
			ImageReader::cleanData(data);
			return false;
		}
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
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;

		const unsigned char* imageData = image.getData();

		GL_ERR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, mIsSRGBCapable ? GL_SRGB8_ALPHA8 : GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));

		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

		GL_ERR_CHECK(glFlush());

		// Update texture width and height attributes
		mSize.x = image.getWidth();
		mSize.y = image.getHeight();
		mFilePath = "Loaded from image"; /** TODO: get image file path from image object. */

		return true;
	}

	bool OpenGLTexture::update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos, unsigned yPos)
	{
		if (mTextureId == 0) {
			return false;
		}

		GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
		GL_ERR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data));

		setSmoothFilter(true);
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
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
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
		OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
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
			OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth));
			GL_ERR_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight));

			if (texWidth != 0 && texHeight != 0) {
				std::vector<unsigned char> pixels(static_cast<size_t>(texWidth) * texHeight * 4);
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

	bool OpenGLTexture::generateMipMaps()
	{
		if (mTextureId == 0U) {
			return false;
		}

		if (OpenGLTexture::sCurrentlyBoundTexture != mTextureId) {
			GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
			OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
		}

		GL_ERR_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
		mHasMipmaps = true;
		GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR));

		return true;
	}

	void OpenGLTexture::setSmoothFilter(bool smoothFilterEnabled)
	{
		if (mIsSmooth != smoothFilterEnabled) {
			mIsSmooth = smoothFilterEnabled;

			if (mTextureId != 0U) {
				// Bind this texture object if needed
				if (OpenGLTexture::sCurrentlyBoundTexture != mTextureId) {
					GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
					OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
				}

				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));
				//GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? (mHasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) : (mHasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST)));
				if (mHasMipmaps) {
					GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR));
				}
				else {
					GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST));
				}
			}
		}
	}

	bool OpenGLTexture::getSmoothFilter() const
	{
		return mIsSmooth;
	}

	void OpenGLTexture::setRepeat(bool repeatEnabled)
	{
		if (mIsRepeated != mIsRepeated) {
			mIsRepeated = repeatEnabled;

			if (mTextureId != 0U) {
				// Bind this texture object if needed
				if (OpenGLTexture::sCurrentlyBoundTexture != mTextureId) {
					GL_ERR_CHECK(glBindTexture(GL_TEXTURE_2D, mTextureId));
					OpenGLTexture::sCurrentlyBoundTexture = mTextureId;
				}

				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
				GL_ERR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
			}
		}
	}

	bool OpenGLTexture::isRepetead() const
	{
		return mIsRepeated;
	}
}
