#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

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
}
