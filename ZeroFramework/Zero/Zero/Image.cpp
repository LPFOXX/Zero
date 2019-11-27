#include <zr_pch.h>

#include "Zero/Core/Core.h"
#include "Zero/Core/Log.h"

#include "Image.h"
#include "ImageReader.h"

namespace zr
{
	Image::Image() :
		mWidth(0U),
		mHeight(0U),
		mChannelCount(0U),
		mFilePath()
	{

	}

	Image::~Image()
	{

	}

	bool Image::create(unsigned width, unsigned height, const glm::uvec4& fillColor)
	{
		if (width > 0 && height > 0) {
			mBuffer.resize(static_cast<size_t>(width) * height * 4U);

			unsigned char* bufferBegin = &mBuffer[0];
			unsigned char* bufferEnd = bufferBegin + static_cast<size_t>(width) * height * 4U;

			unsigned redChannelValue, greenChannelValue, blueChannelValue, alphaChannelValue;

			redChannelValue = clamp(0U, 255U, fillColor.x);
			greenChannelValue = clamp(0U, 255U, fillColor.y);
			blueChannelValue = clamp(0U, 255U, fillColor.z);
			alphaChannelValue = clamp(0U, 255U, fillColor.w);

			while (bufferBegin != bufferEnd) {
				*bufferBegin++ = redChannelValue;
				*bufferBegin++ = greenChannelValue;
				*bufferBegin++ = blueChannelValue;
				*bufferBegin++ = alphaChannelValue;
			}

			mWidth = width;
			mHeight = height;
			mChannelCount = 4U;
			return true;
		}

		return false;
	}

	bool Image::create(unsigned width, unsigned height, const std::vector<unsigned char>& data, unsigned channelCount)
	{
		if (width > 0 && height > 0 && &data[0]) {
			if (channelCount >= 1 && channelCount <= 4) {
				mWidth = width;
				mHeight = height;
				mChannelCount = channelCount;

				createFourChannelImage(width, height, channelCount, &data[0]);

				return true;
			}
		}
		return false;
	}

	bool Image::create(unsigned width, unsigned height, const unsigned char* data, unsigned channelCount)
	{
		if (width > 0 && height > 0 && data) {
			mWidth = width;
			mHeight = height;
			mChannelCount = channelCount;

			unsigned newSize = width * height * 4;

			mBuffer.resize(newSize);
			std::memcpy(&mBuffer[0], data, newSize);

			return true;
		}
		return false;
	}

	bool Image::loadFromFile(const std::string& imageFilePath, bool flipImage, bool force4Channels)
	{
		int width = 0, height = 0, nrChannel = 0;
		unsigned char* data = ImageReader::LoadDataFromFile(imageFilePath, width, height, nrChannel, flipImage, force4Channels);

		if (data) {
			mFilePath = imageFilePath;
			mWidth = width;
			mHeight = height;
			mChannelCount = force4Channels ? 4U : nrChannel;

			// width * height * mChannelCount bytes (one byte per channel) per pixel
			mBuffer.resize(static_cast<size_t>(mWidth) * mHeight * mChannelCount);
			std::memcpy(&mBuffer[0], data, static_cast<size_t>(mWidth) * mHeight * mChannelCount);
			ImageReader::CleanData(data);
			return true;
		}
		return false;
	}

	unsigned Image::getWidth() const
	{
		return mWidth;
	}

	unsigned Image::getHeight() const
	{
		return mHeight;
	}

	unsigned Image::getChannelCount() const
	{
		return mChannelCount;
	}

	const unsigned char* Image::getData() const
	{
		return !mBuffer.empty() ? &mBuffer[0] : nullptr;
	}

	unsigned char* Image::getData()
	{
		return !mBuffer.empty() ? &mBuffer[0] : nullptr;
	}

	const std::string& Image::getPath() const
	{
		return mFilePath;
	}

	bool Image::setPixel(unsigned x, unsigned y, const glm::uvec4& pixelColor)
	{
		// origin (0, 0)
		if (x >= mWidth || x < 0 || y >= mHeight || y < 0) {
			// Beyond image limit
			return false;
		}

		std::size_t index = (static_cast<size_t>(y) * mWidth + x) * 4U;
		mBuffer[index] = pixelColor.x;
		mBuffer[index + 1] = pixelColor.y;
		mBuffer[index + 2] = pixelColor.z;
		mBuffer[index + 3] = pixelColor.w;

		return true;
	}

	bool Image::createFourChannelImage(unsigned width, unsigned height, unsigned channelCount, const unsigned char* data)
	{
		unsigned newSize = width * height * 4U;
		mBuffer.resize(newSize);

		if (channelCount == 4U) {
			std::memcpy(&mBuffer[0], &data[0], newSize);
			return true;
		}

		if (channelCount == 3U) {
			for (unsigned i = 0U; i < width * height; i++) {
				mBuffer[static_cast<size_t>(i) * 4U] = data[i * 3U];
				mBuffer[static_cast<size_t>(i) * 4U + 1U] = data[i * 3U + 1U];
				mBuffer[static_cast<size_t>(i) * 4U + 2U] = data[i * 3U + 2U];
				mBuffer[static_cast<size_t>(i) * 4U + 3U] = 0xFF;
			}
			return true;
		}

		if (channelCount == 2U) {
			for (unsigned i = 0U; i < width * height; i++) {
				mBuffer[static_cast<size_t>(i) * 4U] = data[i * 2U];
				mBuffer[static_cast<size_t>(i) * 4U + 1U] = data[i * 2U + 1U];
				mBuffer[static_cast<size_t>(i) * 4U + 2U] = 0x00;
				mBuffer[static_cast<size_t>(i) * 4U + 3U] = 0xFF;
			}
			return true;
		}

		if (channelCount == 1U) {
			for (unsigned i = 0U; i < width * height; i++) {
				mBuffer[static_cast<size_t>(i) * 4U] = data[i];
				mBuffer[static_cast<size_t>(i) * 4U + 1U] = 0x00;
				mBuffer[static_cast<size_t>(i) * 4U + 2U] = 0x00;
				mBuffer[static_cast<size_t>(i) * 4U + 3U] = 0xFF;
			}
			return true;
		}
		return false;
	}
};
