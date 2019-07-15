#pragma once

namespace zr
{
	enum class DataFormat
	{
		R,		/**< The data contains only one channel. */
		RG,		/**< The data contains only two channel. */
		RGB,	/**< The data contains only three channel. */
		RGBA	/**< The data passed contains all four channels. */
	};

	class Image
	{
	public:
		/**
		* @brief Default constructor.
		*
		*/
		Image();

		/**
		* @brief Default destructor.
		*
		*/
		virtual ~Image();

		/**
		* @brief Creates an image and stores it in memory.
		*
		* Each pixel of the newly created image will be filled with the color in fillColor.
		* Note: a lp::Image mustn't have its height or its width equal to zero. In this case
		* the method returns false.
		*
		* @param width The width of the image.
		* @param height The height of the image.
		* @param fillColor The RGB color the image will be filled with.
		*
		* @return true when successfully create and false otherwise.
		*
		*/
		bool create(unsigned width, unsigned height, const glm::uvec4& fillColor);

		/**
		* @brief Creates an image and stores it in memory.
		*
		* Note: a lp::Image mustn't have its height or its width equal to zero. In this case
		* the method returns false.
		*
		* @param width The width of the image.
		* @param height The height of the image.
		* @param data A vector of bytes containing the image as a 1 dimension buffer.
		* @param channelCount How many data channels data contains.
		*
		* @return true when successfully create and false otherwise.
		*
		*/
		bool create(unsigned width, unsigned height, const std::vector<unsigned char>& data, unsigned channelCount = 4);

		/**
		* @brief Creates an image and stores it in memory.
		*
		* Note: a lp::Image mustn't have its height or its width equal to zero. In this case
		* the method returns false.
		*
		* @param width The width of the image.
		* @param height The height of the image.
		* @param data The address of an array of bytes containing the image as a 1 dimension buffer.
		* @param channelCount How many data channels data contains.
		*
		* @return true when successfully create and false otherwise.
		*
		*/
		bool create(unsigned width, unsigned height, const unsigned char* data, unsigned channelCount = 4);

		bool loadFromFile(const std::string& imageFilePath, bool flipImage = false);

		/**
		* @brief Returns the current width of the image.
		*
		* @return The image width.
		*
		*/
		unsigned getWidth() const;

		/**
		* @brief Returns the current height of the image.
		*
		* @return The image height.
		*
		*/
		unsigned getHeight() const;

		unsigned getChannelCount() const;

		/**
		* @brief Returns pointer to the image data.
		*
		* May return nullptr when there's no data.
		*
		* @return The image data.
		*
		*/
		const unsigned char* getData() const;

		/**
		* @brief Set a pixel color.
		*
		* Set the color of the pixel at position (x,y).
		*
		* @param x The horizontal position of the pixel.
		* @param y The vertical position of the pixel.
		* @param pixelColor The new color of the pixel.
		*
		* @return true when set succesfully, false otherwise.
		*
		*/
		bool setPixel(unsigned x, unsigned y, const glm::uvec4& pixelColor);

	private:
		bool createFourChannelImage(unsigned width, unsigned height, unsigned channelCount, const unsigned char* data);

	private:
		unsigned mWidth;
		unsigned mHeight;
		unsigned mChannelCount;
		std::vector<unsigned char> mBuffer;
	};

	class ImageLoader
	{
	public:
		/**
		* @brief Default destructor.
		*
		*/
		virtual ~ImageLoader();

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
		static unsigned char* loadDataFromFile(const std::string& imagePath, int* width, int* height, int* channelCount, bool flipImage);

		/**
		* @brief Cleans the data loaded previously.
		*
		* @param data The data pointer to clean.
		*
		*/
		static void cleanData(unsigned char* data);
	private:
		/**
		* @brief Default constructor.
		*
		* No object creation for this class.
		*
		*/
		ImageLoader() = delete;
	};

	class ImageWriter
	{
	public:
		/**
		* @brief Default contructor.
		*
		* No object instances can be created for this class.
		*
		*/
		ImageWriter() = delete;

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
		static bool loadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const unsigned char* data);

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
		static bool loadDataIntoFile(const std::string& fileName, unsigned width, unsigned height, const std::vector<unsigned char>& data);

		/**
		* @brief Creates a file from an image.
		*
		* @param fileName The name of the file to be created.
		* @param image The data which is to be loaded into the file.
		*
		* @return true when the file is created successfully, false otherwise.
		*
		*/
		static bool loadDataIntoFile(const std::string& fileName, const Image& image);
	};
}
