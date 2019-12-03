#include <zr_pch.h>

#include "ImageReader.h"
#include "Core/Profiller.h"

#define STB_IMAGE_IMPLEMENTATION
#ifdef _MSC_VER
#define STBI_MSC_SECURE_CRT
#endif
#include <stb_image.h>

namespace zr
{
	ImageReader::~ImageReader()
	{

	}

	unsigned char* ImageReader::LoadDataFromFile(const std::string& imagePath, int& width, int& height, int& channelCount, bool flipImage, bool force4Channels)
	{
		ZR_PROFILER_FUNCTION();

		stbi_set_flip_vertically_on_load(flipImage);
		return stbi_load(imagePath.c_str(), &width, &height, &channelCount, force4Channels ? 4 : 0);
	}

	void ImageReader::CleanData(unsigned char* data)
	{
		stbi_image_free(data);
	}
}
