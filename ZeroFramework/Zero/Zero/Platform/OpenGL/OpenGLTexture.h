#pragma once

#include "../../Renderer/Texture.h"
#include "../../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture();
		OpenGLTexture(const std::string& filePath, TextureType type);
		virtual ~OpenGLTexture();

		// Inherited via Texture
		virtual void bind() const override;
		virtual void bindOnTextureUnit(unsigned textureUnit) override;
		virtual bool loadFromFile(const std::string& fileName, bool flipVertically = false) override;
		virtual bool loadFromFile(const std::string& fileName, Texture::TextureType textureType, bool flipVertically = false) override;
		virtual bool loadFromImage(const Image& image) override;
		virtual bool update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual bool update(const Image& data, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual bool update(const std::vector<unsigned char>& data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) override;
		virtual bool loadToImage(Image& image) const override;
		virtual unsigned getHandle() const override;

	private:
	/**
	 * @brief Sets texture data
	 *
	 * @param width		  	The width.
	 * @param height	  	The height.
	 * @param channelCount	Number of channels.
	 * @param data		  	The data.
	 *
	 * @returns True if it succeeds, false if it fails.
	 */
	bool setTextureData(unsigned width, unsigned height, unsigned channelCount, const unsigned char* data);

	private:
		unsigned mTextureId;		/**< The OpenGL texture handler. */
	};
}

//class Texture
//{
//public:
//	/**
//	 * @brief Copy constructor.
//	 *
//	 * @param otherTexture	The texture to copy from.
//	 */
//	Texture(const Texture& otherTexture);
//
//	/**
//	 * @brief Default destructor.
//	 *
//	 * Tries to delete the opengl texture before destroying the lp::Texture object.
//	 */
//	virtual ~Texture();
//
//	/**
//	 * @brief Assigning constructor.
//	 *
//	 * Assignes the otherTexture to the object being created. It deletes the texture owned by
//	 * otherTexture.
//	 *
//	 * @param otherTexture	The texture to copy from.
//	 *
//	 * @returns The newly created texture.
//	 */
//	Texture& operator=(const Texture& otherTexture);
//
//	
//
//public:
//	/**
//	 * @brief Copies this object
//	 *
//	 * @param 		   src 	Source for the.
//	 * @param [in,out] dest	Destination for the.
//	 */
//	static void copy(const Texture& src, Texture& dest);
//
//	/**
//	 * @brief Gets maximum allowed textue units to bound at once.
//	 *
//	 * @returns The maximum allowed texture units to bound at once.
//	 */
//	static int getMaxAllowedUnitsBound();
//
//private:
//	/**
//	 * @brief Sets texture identifier
//	 *
//	 * @param newTextureId	Identifier for the new texture.
//	 */
//	void setTextureId(unsigned newTextureId) const;
//
//	/**
//	 * @brief Sets texture data
//	 *
//	 * @param width		  	The width.
//	 * @param height	  	The height.
//	 * @param channelCount	Number of channels.
//	 * @param data		  	The data.
//	 *
//	 * @returns True if it succeeds, false if it fails.
//	 */
//	bool setTextureData(unsigned width, unsigned height, unsigned channelCount, const unsigned char* data);
//};

/**
* @class lp::Texture
* @ingroup GraphicEngine
*
* lp::Texture handles an image loadded into graphic memory. It doesn't, however, own the data. It just
* provides ways of loading and unloading image data from OpenGL memory.
*
*/