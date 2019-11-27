#include <zr_pch.h>

#include "ImageReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace zr
{
	ImageReader::~ImageReader()
	{

	}

	unsigned char* ImageReader::LoadDataFromFile(const std::string& imagePath, int& width, int& height, int& channelCount, bool flipImage, bool force4Channels)
	{
		stbi_set_flip_vertically_on_load(flipImage);
		return stbi_load(imagePath.c_str(), &width, &height, &channelCount, force4Channels ? 4 : 0);
	}

	void ImageReader::CleanData(unsigned char* data)
	{
		stbi_image_free(data);
	}
}
