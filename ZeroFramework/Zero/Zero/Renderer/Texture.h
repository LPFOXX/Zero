#pragma once

#include "Zero/Core/Core.h"
#include "Zero/Image.h"

namespace zr
{
	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		/**
		 * @brief Binds this object
		 */
		virtual void bind() const = 0;

		/**
		 * @brief Unbinds this object
		 */
		virtual void unbind() const = 0;
	};

	/**
	 * @brief lp::Texture is a representation of an image object loaded into graphic memory.
	 */
	class Texture2D : public Texture
	{
	public:
		/**
		 * @brief Values that represent texture types.
		 */
		enum class Type
		{
			None = 1,	/**< An enum constant representing the no type option */
			Ambient,	/**< An enum constant representing the ambient option */
			Diffuse,	/**< An enum constant representing the diffuse option */
			Specular,   /**< An enum constant representing the specular option */
			Normal,		/**< An enum constant representing the normal option */
			Height		/**< An enum constant representing the height option */
		};

		/**
		 * @brief Type to string
		 *
		 * @param textureType	Type of the texture.
		 *
		 * @returns A const std::string.
		 */
		static const std::string typeToString(Type textureType);

	public:
		Texture2D();
		virtual ~Texture2D();

		/**
		 * @brief Get the width value of the texture.
		 *
		 * @returns The texture width value.
		 */
		inline virtual float getWidth() const
		{
			return mSize.x;
		}

		/**
		 * @brief Gets the texture height value.
		 *
		 * @returns The texture height value.
		 */
		inline virtual float getHeight() const
		{
			return mSize.y;
		}

		/**
		 * @brief Gets the size
		 *
		 * @returns The size.
		 */
		inline const glm::vec2& getSize() const
		{
			return mSize;
		}

		/**
		 * @brief Gets the type
		 *
		 * @returns The type.
		 */
		inline Type getType() const
		{
			return mTextureType;
		}

		/**
		 * @brief Binds the given texture
		 *
		 * @param texture	The texture.
		 */
		static void Bind(const Texture2D& texture);

		/**
		 * @brief Activates the texture unit.
		 *
		 * textureUnit can be zero. However it must match the value that it is referenced by in the shader
		 * program.
		 *
		 * @param textureUnit	The texture unit.
		 * @param bindTexture	The texture to bind to the texture unit.
		 */
		static void ActivateTextureUnit(unsigned textureUnit, const Texture2D& bindTexture);

		/**
		 * @brief Activates the texture unit.
		 *
		 * textureUnit can be zero. However it must match the value that it is referenced by in the shader
		 * program.
		 *
		 * @param textureUnit	The texture unit.
		 * @param bindTexture	The texture to bind to the texture unit.
		 */
		static void ActivateTextureUnit(unsigned textureUnit, unsigned bindTexture);

		/**
		 * @brief Bind on texture unit.
		 *
		 * @param textureUnit	The texture unit to bind this texture object to.
		 */
		virtual void bindOnTextureUnit(unsigned textureUnit) = 0;

		/**
		 * @brief Loads from file
		 *
		 * @param fileName			Filename of the file.
		 * @param flipVertically	(Optional) True to flip vertically.
		 * @param textureType   	Type of the texture.
		 *
		 * @returns True if it succeeds, false if it fails.
		 */
		virtual bool loadFromFile(const std::string& filePath, bool flipVertically = false, Type textureType = Type::None) = 0;

		/**
		 * @brief Loads the texture data from an lp::Image.
		 *
		 * Checks for image existence and the validity of the data before add it to the texture.
		 *
		 * @param image	The image to load the data from.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool loadFromImage(const Image& image, Type textureType = Type::None) = 0;

		/**
		 * @brief Loads the texture data from a memory address.
		 *
		 * @param width	The width of the image in the specified address.
		 * @param width	The heigth of the image in the specified address.
		 * @param data	The address where the pixel data is stored.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool loadFromMemory(float width, float height, const unsigned char* data) = 0;

		/**
		 * @brief Update the texture data.
		 *
		 * If data is a valid data pointer it will be loaded to the texture data in the positions given
		 * in xPos and yPos.
		 *
		 * @param data  	The data pointer to update the texture data with.
		 * @param width 	The width of the data.
		 * @param height	The height of the data.
		 * @param xPos  	(Optional) The horizontal offset in the texture to load data into.
		 * @param yPos  	(Optional) The vertical offset in texture to load data into.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool update(const unsigned char* data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) = 0;

		/**
		 * @brief Update the texture data.
		 *
		 * If data hold by the image is a valid data pointer it will be loaded to the texture data in
		 * the positions given in xPos and yPos.
		 *
		 * The width and height of the image is retrieved in the image object itself.
		 *
		 * @param data	The image which data pointer is to update the texture data with.
		 * @param xPos	(Optional) The horizontal offset in the texture to load data into.
		 * @param yPos	(Optional) The vertical offset in texture to load data into.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool update(const Image& data, unsigned xPos = 0, unsigned yPos = 0) = 0;

		/**
		 * @brief Update the texture data.
		 *
		 * This method get the data to load from a vector of bytes. If data is a valid data pointer it
		 * will be loaded to the texture data in the positions given in xPos and yPos.
		 *
		 * @param data  	The data pointer to update the texture data with.
		 * @param width 	The width of the data.
		 * @param height	The height of the data.
		 * @param xPos  	(Optional) The horizontal offset in the texture to load data into.
		 * @param yPos  	(Optional) The vertical offset in texture to load data into.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool update(const std::vector<unsigned char>& data, unsigned width, unsigned height, unsigned xPos = 0, unsigned yPos = 0) = 0;

		virtual void setSmooth(bool smoothFilterEnabled) = 0;

		virtual inline bool isSmooth() const
		{
			return mIsSmooth;
		}

		virtual void setRepeat(bool repeatEnabled) = 0;

		virtual inline bool isRepetead() const
		{
			return mIsRepeated;
		}

		virtual inline bool hasMipMaps() const
		{
			return mHasMipmaps;
		}

		virtual bool generateMipMaps() = 0;

		virtual void invalidateMipmaps() = 0;

		/**
		 * @brief Loads the data contained in the texture into a lp::Image.
		 *
		 * @param [in,out] image	The image where the data is going to be loaded.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool loadToImage(Image& image) const = 0;

		/**
		 * @brief Gets the handle of the texture generated by the OpenGL state machine.
		 *
		 * When the handle returned is different from zero it is a valid texture, otherwise it is not.
		 *
		 * @returns The identifier of texture inside the OpenGL state machine.
		 */
		virtual unsigned getHandle() const = 0;

		/**
		 * @brief Gets file path
		 *
		 * @returns The file path.
		 */
		inline const std::string getFilePath() const
		{
			return mFilePath;
		}

		/**
		 * @brief Queries the OpenGL state machine about the maximum allowed texture size.
		 *
		 * @returns The maximum texture size supported by OpenGL.
		 */
		static int GetMaximumSize();

		static Ref<Texture2D> Create();

		static Ref<Texture2D> Create(const std::string& filePath, Type type);

	protected:
		glm::vec2 mSize;			/**< The texture size. */
		Type mTextureType;			/**< The type of the texture. */
		std::string mFilePath;		/**< The path of the texture file. Used to prevent loading it more than once when it is an ASSIMP material texture. */
		bool mIsSRGBCapable;		/**< Whether this texture is sRGB capable or not. */
		bool mIsSmooth;
		bool mIsRepeated;
		bool mHasMipmaps;
	};
}
