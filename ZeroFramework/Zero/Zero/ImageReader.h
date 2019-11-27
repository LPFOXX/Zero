#pragma once

namespace zr
{
	class ImageReader
	{
	public:
		/**
		* @brief Default destructor.
		*
		*/
		virtual ~ImageReader();

		/**
		* @brief Load the data from a file.
		*
		* Forces 4 channels output. Meaning that even though the image doesn't have 4 channels
		* stb_image will make sure the output data has.
		*
		* @param imagePath The path of the image file.
		* @param width The windth of the file loaded.
		* @param height The height of the file loaded.
		* @param channelCount The amount of channels for each pixel of the image.
		* @param flipImage Whether or not to flip the image vertically.
		*
		* @return The data loaded from the file. If it is NULL then image loading failed.
		*
		*/
		static unsigned char* LoadDataFromFile(const std::string& imagePath, int& width, int& height, int& channelCount, bool flipImage, bool force4Channels = false);

		/**
		* @brief Cleans the data loaded previously.
		*
		* @param data The data pointer to clean.
		*
		*/
		static void CleanData(unsigned char* data);
	private:
		/**
		* @brief Default constructor.
		*
		* No object creation for this class.
		*
		*/
		ImageReader() = delete;
	};
}
