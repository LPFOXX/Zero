#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H

#include "../Core/Core.h"
#include "../Image.h"

#include "Texture.h"

namespace zr
{
	/**
	* @brief zr::Font represents a loaded font that can be used to generate text.
	*
	* This is a heavyweight object and should be used only to provide funtions to display text.
	* It only needs to be instanciated and passed to the lp::Text object.
	*
	* @see lp::Text
	*
	*/
	class Font
	{
	public:
		struct Character
		{
			Character() :
				Size(0, 0),
				Bearing(0, 0),
				Advance(0.f),
				CharacterRectOrigin(0, 0)
			{
			}

			glm::vec2 Size;				/**< Size of the glyph.	*/
			glm::vec2 Bearing;				/**< Offset from baseline. */
			float Advance;					/**< Offset to advance to the next glyph. */
			glm::vec2 CharacterRectOrigin;	/**< Position of the begining of the glyph in the texture. */
		};

		using Characters = std::map<unsigned long long, Character>;

		/**
		* @brief A struct representation of a row of characters in the texture containing
		* glyphs of a given font size.
		*
		*/
		struct Row
		{
			/**
			* @brief Default constructor.
			*
			*/
			Row() :
				VerticalOffset(0U),
				LastHorizontalOffset(0U),
				Height(0U)
			{

			}

			unsigned VerticalOffset;			/**< Vertical offset of the row counting from texture origin. */
			unsigned LastHorizontalOffset;		/**< Offset of the last glyph in the row. Next glyph will be inserted from here. */
			unsigned Height;					/**< Height of the row. */
		};

		/**
		* @brief Typedef renaming a vector of zr::Rows.
		*
		*/
		typedef std::vector<Row> Rows;

		/**
		* @brief A struct defining a texture containing glyphs representing characters of a given
		* font size.
		*
		*/
		struct Page
		{
			/**
			* @brief Default constructor.
			*
			*/
			Page() :
				CharacterMap(),
				Texture(),
				Rows(),
				NextRowVerticalOffset(0)
			{
				Image image;
				image.create(128, 128, glm::uvec4(255, 255, 255, 0));

				Texture = Texture2D::Create();
				Texture->loadFromImage(image);
				Texture->setSmooth(true);
			}

			Characters CharacterMap;			/**< A map containg character information that are already loaded. */
			Ref<Texture2D> Texture;				/**< The texture containing the glyphs loaded from the font file. */
			Rows Rows;							/**< Information about the rows of glyphs in the texture. */
			unsigned NextRowVerticalOffset;		/**< The offset of the next available row to add glyphs into in the texture. */
		};

		/**
		* @brief The typedef defining a map of pages.
		*
		*/
		typedef std::map<unsigned, Page> PageMap;

	public:
		/**
		* @brief Default constructor.
		*
		*/
		Font();

		/**
		* @brief Default destructor.
		*
		*/
		virtual ~Font();

		/**
		* @brief Loads a font from a file.
		*
		* @param fontFilePath The path of the file containing the font.
		*
		* @return true when loaded successfully, false otherwise.
		*
		*/
		bool loadFromFile(const std::string& fontFilePath);

		/**
		* @brief Queries for a character identified by codePoint from the font file.
		*
		* It may return an 'empty' character. That is everything se to zero. This can happen when
		* the font doesn't contain the character or there has been an error loading the font.
		*
		* @param codePoint The unicode identifier of the character.
		* @param fontSize The size of the font to query.
		*
		* @return The handle to the character info.
		*
		*/
		const Character& getCharacter(unsigned codePoint, unsigned fontSize, unsigned outlineThickness = 0U) const;

		/**
		* @brief Gets the width of the texture that contains the characters of fontSize.
		*
		* @param fontSize The font size of the texture requested.
		*
		* @return The width of the texture when it exists, 0 when it doesn't.
		*
		*/
		unsigned getTextureWidth(unsigned fontSize) const;

		/**
		* @brief Gets a reference to the texture that contains the character set of the given font size.
		*
		* @param fontSize The font size of the character set hold by the texture requested.
		*
		* @return A reference to the texture requested.
		*
		*/
		const Ref<Texture2D>& getTexture(unsigned fontSize) const;

		/**
		* @brief Gets the distance that should be added or taken from the normal advance of the two given characters.
		*
		* @param first The codepoint of the first character in the pair.
		* @param second The codepoint of the second character in the pair.
		* @param fontSize The size of the font to get the kerning value from.
		*
		* @return The value in pixels to be added to the normal advance of the character. It can be negative.
		*
		*/
		float getKerning(unsigned first, unsigned second, unsigned fontSize) const;

		float getLineSpacing(unsigned fontSize) const;

	private:
		/**
		* @brief Gets a character from the font file and stores it for futher use.
		*
		* When there's an error with Freetype library initialization the character returned is
		* an empty one - meaning everithing is set to default values (zero most of the times).
		*
		* This method gets the character with sontSize from the font file, gets the bitmap representation
		* of the character and adds it to the texture containing the characters of size equal to fontSize.
		*
		* @param codePoint			The identification of the character in the font file.
		* @param fontSize			The font size of the character to load.
		* @param outlineThickness	The font outline thickness.
		*
		* @return The character requested. It can be an 'empty' character, though.
		*
		*/
		const Character loadCharacter(unsigned codePoint, unsigned fontSize, unsigned outlineThickness) const;

		/**
		* @brief Adds a character to the texture.
		*
		* Before adding the character to the texture it is queried where to put it. In case
		* there is no more space for it, a bigger texture is created and the character is added
		* to it alongside with the old ones.
		*
		* @param page The page containing the characters of a given size.
		* @param character The character to insert into the texture.
		*
		* @return true when added successfully, false otherwise.
		*
		*/
		bool addCharacterToTexture(Page& page, Character& character, unsigned outlineThickness) const;

		/**
		* @brief Sets the font size.
		*
		* This method is called when the font size of the font is changed. It is so because the
		* underlying library only has a font size loaded at a time.
		*
		* @param fontSize The size of the font to change to.
		*
		* @return true when the font size is changed successfully, false otherwise.
		*
		*/
		bool setCharSize(unsigned fontSize) const;

		/**
		* @brief Generates a unique key to reference a character.
		*
		* Theres a need to store the identifier of a non-outlined character and its outline counterpart in the same
		* character atlas/page (and the texture as well).
		*
		* @param codePoint			The character/glyph.
		* @param outlineThickness	The outline thickness for the font.
		*
		* @return					The combination of the the parameter in a 64 bit number.
		*
		*/
		unsigned long long generateCharacterKey(unsigned codePoint, unsigned outlineThickness) const;

	private:
		FT_Library mFreetypeLibrary;	/**< The font library handle. */
		FT_Face mFontFace;				/**< The identifier/handle of the font. */
		FT_Stroker mStroker;			/**< A stroker object to draw character/glyph outlines. */
		mutable PageMap mPages;			/**< A set of a texture and informations about the bitmap of a given font size. */

		mutable std::vector<unsigned char> mPixelBuffer;	/**< Temporary storage of bitmaps of characters requested to the font library. */
	};
}
