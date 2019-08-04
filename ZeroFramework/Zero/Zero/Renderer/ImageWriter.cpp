#include <zr_pch.h>

#include "ImageWriter.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#ifdef _MSC_VER
//#define STBI_MSC_SECURE_CRT
//#endif
#include <stb_image_write.h>

namespace zr
{
	ImageWriter::~ImageWriter()
	{
	}

	bool ImageWriter::LoadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const unsigned char* data)
	{
		if (data) {
			return stbi_write_png(fileName.c_str(), width, height, 4, data, 0);;
		}
		return false;
	}

	bool ImageWriter::LoadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const std::vector<unsigned char>& data)
	{
		return ImageWriter::LoadDataIntoFile(fileName, width, height, &data[0]);
	}

	bool ImageWriter::LoadDataIntoFile(const std::string& fileName, const Image& image)
	{
		return ImageWriter::LoadDataIntoFile(fileName, image.getWidth(), image.getHeight(), image.getData());
	}
}
