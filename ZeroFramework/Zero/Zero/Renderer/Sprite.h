#pragma once

#include "Drawable2D.hpp"

#include "Transformable.hpp"
#include "Resizable.h"
#include "ViewportDependable.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"

namespace zr
{
	class Sprite2D : public Transformable2D, public Drawable2D
	{
	public:
		Sprite2D() :
			Transformable2D(),
			Drawable2D()
		{

		}

		virtual ~Sprite2D() = default;

		void setTexture(const Ref<Texture>& texture)
		{
			mTexture = texture;
		}

		void setTextureCoordinates()
		{

		}

		// Inherited via Drawable2D
		virtual void transformVertices() const override;

	protected:
		Ref<Texture> mTexture;
	};

	/**
	 * @brief lp::Sprite represents a 2D image to be drawn.
	 */
	class Sprite : public Movable2D, public Resizable, public ViewportResizeListener
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Creates a sprite object and load the rect vertices positions.
		 *
		 * @param loadDefaultShader	(Optional) Whether or not to load the sprite default shader. It is
		 * 							loaded by default.
		 */
		Sprite(bool loadDefaultShader = true);

		/**
		 * @brief Default destructor.
		 */
		virtual ~Sprite();

		/**
		 * @brief Sets the texture that will be drawn by the lp::Sprite object.
		 *
		 * @param [in,out] texture	The texture to set.
		 */
		void setTexture(const Ref<Texture2D>& texture);

		/**
		 * @brief Sets the position and the size of the rect displaying the sprite.
		 *
		 * Origin (0,0) is at top left of the viewport.
		 *
		 * @param rect	The rect containing the sprite.
		 */
		void setRect(const glm::uvec4& rect);

		/**
		 * @brief Sets the position and the size of the rect displaying the sprite.
		 *
		 * Origin (0,0) is at top left of the viewport.
		 *
		 * @param xPosition	Horizontal position of the sprite.
		 * @param yPosition	Vertical position of the sprite.
		 * @param width	   	The sprite width.
		 * @param height   	The sprite height.
		 */
		void setRect(int xPosition, int yPosition, unsigned width, unsigned height);

		/**
		 * @brief Sets the position and the size of the rect displaying the sprite.
		 *
		 * Origin (0,0) is at top left of the viewport.
		 *
		 * @param position	The position of the rect containing the sprite.
		 * @param size	  	The size of the rect containing the sprite.
		 */
		void setRect(const glm::uvec2& position, const glm::uvec2& size);

		/**
		 * @brief Sets the texture coordinates of the quad containing a frame of an animation.
		 *
		 * All values are referenced in pixels.
		 *
		 * @param texPositionX	The horizontal position of the frame quad within the image.
		 * @param texPositionY	The vertical position of the frame quad within the image.
		 * @param width		  	The width of the frame.
		 * @param height	  	The height of the frame.
		 */
		void setTextureCoordinates(unsigned texPositionX, unsigned texPositionY, unsigned width, unsigned height);

		const glm::vec4 getRect() const;

		void setColor(const glm::vec4& color);

		void setColorOpacity(float opacity);

		void setBorderThickness(unsigned borderThickness);

		void setBorderColor(const glm::vec4& borderColor);

		/**
		 * @brief Displays the sprite into the viewport.
		 */
		virtual void render(const glm::mat4& viewProjectionMatrix);

	protected:
		/**
		 * @brief Updates the vertices according to the viewport size.
		 */
		void updateVertices();

		/**
		 * @brief Inherited via Resizable
		 */
		virtual void onSizeUpdate() override;

		/**
		 * @brief Inherited via ViewportResizeListener
		 */
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override;

	protected:
		Ref<VertexArray> mSpriteVAO;		/**< Handler to the vertex array object. */
		Ref<Shader> mShader;				/**< The shader to draw the sprite with. */
		Ref<Texture2D> mTexture;				/**< The texture reference. */
		glm::vec4 mColor;								/**< The color of the sprite. */
		std::vector<float> mTexCoordinates; /**< A 2x4 array of values representing the texture coordinates of part of the texture to draw. */

		std::vector<float> mVertices;	/**< The vertices of the sprite. */
		bool mNeedsUpdate;				/**< A flag that represent the updating state of the vertices. */

	private:
		static const std::string sVertexShader;		/**< The string containing the vertex shader. */
		static const std::string sFragmentShader;	/**< The string containing the fragment shader. */

	private:
		bool mDrawingTexture = false;
		float mViewportHeight;
		//Border mBorder;
	};
};

/**
* @class lp::Sprite
* @ingroup GraphicEngine
*
*/
