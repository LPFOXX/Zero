#include <zr_pch.h>

#include "ImageReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace zr
{
	ImageReader::~ImageReader()
	{

	}

	unsigned char* ImageReader::loadDataFromFile(const std::string& imagePath, int& width, int& height, int& channelCount, bool flipImage)
	{
		stbi_set_flip_vertically_on_load(flipImage);
		// Force 4 channels output by specifying 4 as the last parameter
		return stbi_load(imagePath.c_str(), &width, &height, &channelCount, 4);
	}

	void ImageReader::cleanData(unsigned char* data)
	{
		stbi_image_free(data);
	}
}
