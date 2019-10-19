#pragma once

#include "Image.h"

namespace zr
{
	class ImageWriter
	{
	public:
		/**
		* @brief Default destructor.
		*
		*/
		virtual ~ImageWriter();

		/**
		* @brief Creates a file from the raw data pointer.
		*
		* @param fileName The name of the file to be created.
		* @param width The width of the image to create.
		* @param height The height of the image to create.
		* @param data The data which is to be loaded into the file.
		*
		* @return true when the file is created successfully, false otherwise.
		*
		*/
		static bool LoadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const unsigned char* data);

		/**
		* @brief Creates a file from the data from a vector.
		*
		* @param fileName The name of the file to be created.
		* @param width The width of the image to create.
		* @param height The height of the image to create.
		* @param data The data which is to be loaded into the file.
		*
		* @return true when the file is created successfully, false otherwise.
		*
		*/
		static bool LoadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const std::vector<unsigned char>& data);

		//* @brief Creates a file from an image.
		//*
		//* @param fileName The name of the file to be created.
		//* @param image The data which is to be loaded into the file.
		//*
		//* @return true when the file is created successfully, false otherwise.
		//*
		//*/
		static bool LoadDataIntoFile(const std::string& fileName, const Image& image);

	private:
		/**
		* @brief Default contructor.
		*
		* No object instances can be created for this class.
		*
		*/
		ImageWriter() = delete;
	};
}
