#pragma once

#include "../Movable.h"
#include "ViewportDependable.h"
#include "Font.h"
#include "Buffer.h"
#include "Shader.h"

namespace zr
{
	/**
	 * @brief lp::Text is a visual representation of a string of text.
	 *
	 * Handles positioning, color and size of text displayed.
	 */
	class Text : public Movable, public ViewportResizeListener
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Text();

		/**
		 * @brief Constructor.
		 *
		 * Builds a text from a string, the font and the font size.
		 *
		 * @param string		  	The string to be displayed.
		 * @param font			  	The font reference of the object containing the font.
		 * @param fontSize		  	(Optional) The size of the font to display the text with.
		 * @param outlineThickness	(Optional) The outline thickness.
		 */
		Text(const std::string& string, const std::shared_ptr<Font>& font, unsigned fontSize = 30U, unsigned outlineThickness = 0U);

		/**
		 * @brief Default destructor.
		 */
		virtual ~Text();

		/**
		 * @brief Set the font that will be used to display the text with.
		 *
		 * The lp::Font object used mustn't be destroyd before destroybd this lp::Text object.
		 *
		 * @param font	A reference to the lp::Font object.
		 *
		 * @sa lp::Font
		 */
		void setFont(const std::shared_ptr<Font>& font);

		/**
		 * @brief Sets the text to display.
		 *
		 * @param string	The text to display.
		 */
		void setString(const std::string& string);

		/**
		 * @brief Sets the number value to display.
		 *
		 * @param value	The number (unsigned integer) to display.
		 */
		void setNumber(unsigned value);

		/**
		 * @brief Sets the font size.
		 *
		 * Sets the font size to display the text in.
		 *
		 * @param fontSize	The size of the font to display the text in.
		 */
		void setFontSize(unsigned fontSize);

		/**
		 * @brief Sets the color to display the text.
		 *
		 * The color channels are in the [0-255] interval. If any of them are greater of less than both
		 * interval max the value is clamped.
		 *
		 * @param color	The color to display the text in.
		 */
		void setFillColor(const glm::vec4& color);

		/**
		 * @brief Sets the value of the outline tickness for the text (i.e. character border).
		 *
		 * @param outlineThickness	The value of the outline tickness (i.e. The text border).
		 */
		void setOutlineThickness(unsigned outlineThickness);

		/**
		 * @brief Sets the outline color value.
		 *
		 * @param color	The color to update the outline color to.
		 */
		void setOutlineColor(const glm::vec4& color);

		void render(const glm::mat4& viewProjectionMatrix);

		/**
		 * @brief Addition operator.
		 * Append the content in value to the content stored by this object.
		 *
		 * @param value	The value to append.
		 */
		template<typename T>
		void operator+(const T& value);

		template<typename T>
		void operator<<(const T& value);

		/**
		 * @brief Gets the position and size of the text quad.
		 *
		 * @returns A named vector containing the position and size of the quad containing the text.
		 */
		virtual const glm::vec2& getSize() const;

	protected:
		/**
		 * @brief Inherited via Movable
		 */
		virtual void onPositionUpdate() override;

		virtual void onSizeUpdate();

	private:
		/**
		 * @brief Initializes the static objects
		 */
		static void initializeStaticObjects();

	private:
		/**
		 * @brief Dry run
		 */
		void dryRun();
		/**
		 * @brief Updates the projection matrix
		 */
		//void updateProjectionMatrix();

	protected:
		mutable glm::vec2 mQuadSize;			/**< The size of the quad containing the text. */
		unsigned mOutlineThickness;				/**< The value of the text outline in pixels. */

	private:
		static unsigned sReferenceCount;		/**< Number of references currently instanciated by this class. */
		static std::string sVertexShader;		/**< The vertex shader string of the text/quad. */
		static std::string sFragmentShader;		/**< The fragment shader string of the text/quad. */

		static std::shared_ptr<VertexArray> sVAO;	/**< A handle to the vertex array object. */
		static std::shared_ptr<Shader> sShader;		/**< A wrapper to a shader program to communicate to the OpenGL state machine. */
		static bool sIsInitialized;					/**< Whether or not the static objects (VAO, VBO, EBO) are initialized. */
		static unsigned sMaxCharactersToDraw;		/**< Maximum characteres to draw. */

	private:
		float mViewportHeight;
		std::shared_ptr<Font> mFont;			/**< The font object reference. */
		unsigned mFontSize;						/**< The size of the font. */
		std::string mString;					/**< The text to display. */
		bool mNeedsUpdate = true;
		
		std::vector<float> mFillVertices;
		std::vector<float> mOutlineVertices;

		glm::vec4 mColor;						/**< The color of the text to display. */
		glm::vec4 mOutlineColor;				/**< The outline color. */

		float mMaxVerticalBearing = 0.f;
		// Inherited via ViewportResizeListener
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override;
		/**< The maximum vertical bearing. */
	};

	template<typename T>
	inline void Text::operator+(const T& value)
	{
		mString += std::to_string(value);
		dryRun();
	}

	template<typename T>
	inline void Text::operator<<(const T& value)
	{
		mString += std::to_string(value);
		dryRun();
	}
}

/**
* @class zr::Text
* @ingroup GraphicEngine
*
* lp::Text represents a string of text to be rendered to the viewport given a position and a color attribute.
*
*/