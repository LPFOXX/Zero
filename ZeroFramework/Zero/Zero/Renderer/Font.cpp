#include <zr_pch.h>

#include "Font.h"

#include "../Core/Log.h"

namespace zr
{
	Font::Font() :
		mFreetypeLibrary(nullptr),
		mFontFace(nullptr),
		mStroker(nullptr)
	{

	}

	Font::~Font()
	{
		if (mFontFace) {
			// Don't need the font anymore, so tell freetype for free it
			if (!FT_Done_Face(mFontFace)) {
				mFontFace = nullptr;
			}
		}

		if (mStroker) {
			// Done with the stroker
			FT_Stroker_Done(mStroker);
		}

		if (mFreetypeLibrary) {
			// Destroys Freetype instance
			if (!FT_Done_FreeType(mFreetypeLibrary)) {
				mFreetypeLibrary = nullptr;
			}
		}
	}

	bool Font::loadFromFile(const std::string& fontFilePath)
	{
		// TODO: get Boost Filesystem back
		//namespace fs = boost::filesystem;
		//fs::path fontPath(fontFilePath);
		/*std::string filePath = fontPath.string();
		if (!fs::exists(fontPath)) {
			return false;
		}*/

		// Initialize font FreeType
		if (FT_Init_FreeType(&mFreetypeLibrary)) {
			ZR_CORE_ERROR("[FREETYPE] Couldn't initialize FreeType library.");
			return false;
		}

		std::ifstream file(fontFilePath);
		if (!file.good()) {
			file.close();
			ZR_CORE_ERROR("[FREETYPE] Couldn't read font file.");
			return false;
		}
		file.close();
		
		// Initialize the font face from the specified file
		if (FT_New_Face(mFreetypeLibrary, fontFilePath.c_str(), 0, &mFontFace)) {
			ZR_CORE_ERROR("FREETYPE ERROR: Couldn't initialize a FreeType face object.");
			FT_Done_FreeType(mFreetypeLibrary);
			return false;
		}

		// Create the stroker object
		if (FT_Stroker_New(mFreetypeLibrary, &mStroker)) {
			// Error creating stroker object
			ZR_CORE_ERROR("FREETYPE ERROR: Couldn't create stroker object.");
			// Destroy font face
			FT_Done_Face(mFontFace);
			// Destroy library
			FT_Done_FreeType(mFreetypeLibrary);

			return false;
		}

		// Request the charset for the font
		if (FT_Select_Charmap(mFontFace, FT_ENCODING_UNICODE)) {
			ZR_CORE_ERROR("FREETYPE ERROR: Couldn't set Unicode charset for the font.");
			// Destroy font face
			FT_Done_Face(mFontFace);
			//Destroy library
			FT_Done_FreeType(mFreetypeLibrary);

			return false;
		}

		return true;
	}

	const Font::Character& Font::getCharacter(unsigned codePoint, unsigned fontSize, unsigned outlineThickness) const
	{
		// Search for the page that contains the characters of fontSize
		Characters& characters = mPages[fontSize].CharacterMap;

		unsigned long long key = generateCharacterKey(codePoint, outlineThickness);

		auto it = characters.find(key);
		if (it != characters.end()) {
			// Glyph found, then return it
			return it->second;
		}
		else {
			// Not found: load it
			Character character = loadCharacter(codePoint, fontSize, outlineThickness);
			return characters.insert(std::make_pair(key, character)).first->second;
		}
	}

	float Font::getTextureWidth(unsigned fontSize) const
	{
		const Page& page = mPages[fontSize];

		if (page.Texture) {
			return page.Texture->getWidth();
		}
		return 0.f;
	}

	const Ref<Texture2D>& Font::getTexture(unsigned fontSize) const
	{
		return mPages[fontSize].Texture;
	}

	const Font::Character Font::loadCharacter(unsigned codePoint, unsigned fontSize, unsigned outlineThickness) const
	{
		Font::Character character;
		// Check for font initialization
		if (!mFontFace) {
			return character;
		}

		// Try to change the current font size
		if (!setCharSize(fontSize)) {
			return character;
		}

		// Set loading flags
		FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;

		if (outlineThickness != 0U) {
			flags |= FT_LOAD_NO_BITMAP;
		}

		// Get the character index (as mapped by the font)
		FT_UInt charIndex = FT_Get_Char_Index(mFontFace, codePoint);

		if (!charIndex) {
			// When charIndex is zero, meaning that it is an 'unknown character code'
			return character;
		}

		// Load the glyph corresponding to the code point
		if (FT_Load_Glyph(mFontFace, charIndex, flags)) {
			return character;
		}

		FT_Glyph glyphDesc;
		if (FT_Get_Glyph(mFontFace->glyph, &glyphDesc)) {
			return character;
		}

		// Make sure the glyph is an outline
		bool outline = (outlineThickness != 0U) && (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
		if (outline) {
			// outline << 6 -> computing the outline tickness radius in 16.16 fixed point format 
			FT_Stroker_Set(mStroker, static_cast<FT_Fixed>(static_cast<float>(outlineThickness) * static_cast<float>(1 << 6)), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
			FT_Glyph_Stroke(&glyphDesc, mStroker, true);
		}

		// Try to get the bitmap for the character in a 8bit antialised format
		FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_Bitmap bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;

		// The width and the height of the bitmap containing the character/glyph
		unsigned width = bitmap.width;
		unsigned height = bitmap.rows;

		// The offset from the origin of the character position
		unsigned bitmapleft = mFontFace->glyph->bitmap_left;
		unsigned bitmapTop = mFontFace->glyph->bitmap_top;

		character.Size = glm::ivec2(width, height);
		character.Bearing = glm::ivec2(bitmapleft, bitmapTop);
		character.Advance = static_cast<float>(mFontFace->glyph->advance.x >> 6);

		if (codePoint != ' ') {
			const unsigned char* glyphBitmap = bitmap.buffer;

			// Resize buffer to accommodate the pixel data from the loaded character
			mPixelBuffer.resize(static_cast<size_t>(width) * height * 4);

			// Set the colors of the pixel buffer to transparent white
			// An iterator could be used but working with the memory itself seems to be faster
			unsigned char* bufferBegin = &mPixelBuffer[0];
			unsigned char* bufferEnd = bufferBegin + static_cast<size_t>(width) * height * 4;

			while (bufferBegin != bufferEnd) {
				(*bufferBegin++) = 255;		// red channel
				(*bufferBegin++) = 255;		// green channel
				(*bufferBegin++) = 255;		// blue channel
				(*bufferBegin++) = 0;		// alpha channel
			}

			// Put data into the object pixel buffer
			if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
				// Pixels are 1 bit monochrome values
				if (!glyphBitmap) {
					// TODO: might incur an error later
					ZR_CORE_INFO("[FONT] Can't load glyph bitmap.");
				}
				else {
					for (unsigned y = 0; y < height; y++) {
						for (unsigned x = 0; x < width; x++) {
							std::size_t index = static_cast<size_t>(width) * y + x;

							bool bit = glyphBitmap[x / 8] & (1 << (7 - (x % 8)));

							// Monochrome values represent 2 alpha channel levels
							unsigned char byte = bit ? 255 : 0;

							// This byte represents the alpha channel so each pixel begins at every 4 bytes
							// and the alpha channel begins at the 4 (last) byte
							mPixelBuffer[index * 4 + 3] = byte;
						}
						// Advance pointer
						glyphBitmap += bitmap.pitch;
					}
				}
			}
			else {
				// Pixels are 8 bits gray levels
				if (!glyphBitmap) {
					// TODO: might incur an error later
					ZR_CORE_INFO("[FONT] Can't load glyph bitmap.");
				}
				else {
					for (unsigned y = 0; y < height; y++) {
						for (unsigned x = 0; x < width; x++) {
							std::size_t index = static_cast<size_t>(width) * y + x;
							unsigned char byte = glyphBitmap[index];

							// This byte represents the alpha channel. Each pixel begins at every 4 bytes
							// and the alpha channel begins at the 4 (last) byte (offset 3)
							mPixelBuffer[index * 4 + 3] = byte;
						}
					}
				}
			}
		}

		Page& page = mPages[fontSize];

		// When the character is a space it wont make difference because its bitmap is
		// a 0x0 quad.
		if (codePoint != ' ') {
			addCharacterToTexture(page, character, outlineThickness);
		}

		// Done with the glyph
		FT_Done_Glyph(glyphDesc);

		return character;
	}

	bool Font::addCharacterToTexture(Page& page, Character& character, unsigned outlineThickness) const
	{
		glm::vec2 characterOffset(0.f, 0.f);

		float characterWidth = character.Size.x;
		float characterHeight = character.Size.y;
		float padding = 2.f + (float)outlineThickness * 2.f; // padding to apply around the character

		Rows& rows = page.Rows;
		Ref<Texture2D>& texture = page.Texture;
		// Check whether there are any rows in the page texture
		if (rows.empty()) {
			// If it is empty, make a new row
			Row row;

			// Set its vertical offset
			row.VerticalOffset = 0.f;

			// Set its size based on the character size + the padding
			float rowHeight = characterHeight + 2 * padding;
			row.Height = rowHeight;

			// Set the vertical offset of the next row
			page.NextRowVerticalOffset = rowHeight;

			// Add the character to the texture
			texture->update(mPixelBuffer, (unsigned)characterWidth, (unsigned)characterHeight, (unsigned)padding, (unsigned)padding);

			// Save character position in texture
			characterOffset.x = padding; // Horizontal offset from texture origin
			characterOffset.y = padding; // Vertical offset from texture origin

			character.CharacterRectOrigin = characterOffset;

			row.LastHorizontalOffset = characterWidth + padding * 2.f;
			rows.push_back(row);
		}
		else {
			// Check for best row
			float textureWidth = texture->getWidth();
			float textureHeight = texture->getHeight();

			auto it = rows.begin();
			auto end = rows.end();

			for (; it != end; ++it) {
				Row& row = *it;

				// Try to add the character to this row
				// The character doesn't fit the row because the row is too small
				if (row.Height < (characterHeight + 2.f * padding)) {
					continue;
				}

				// Try to add the character to the end of the row
				// There's no space to the character at the end of the row
				if (row.LastHorizontalOffset + characterWidth + 2.f * padding > textureWidth) {
					continue;
				}

				// All tests passed for this row
				// Add the character to it
				float rowVerticalOffset = row.VerticalOffset;
				float rowHorizontalOffset = row.LastHorizontalOffset;

				texture->update(mPixelBuffer, (unsigned)characterWidth, (unsigned)characterHeight, (unsigned)(rowHorizontalOffset + padding), (unsigned)(rowVerticalOffset + padding));

				characterOffset.x = rowHorizontalOffset + padding;
				characterOffset.y = rowVerticalOffset + padding;

				character.CharacterRectOrigin = characterOffset;

				row.LastHorizontalOffset += characterWidth + 2.f * padding;

				break;
			}

			if (it == end) {
				// The for loop reached the end meaning that there were no existing rows
				// to add the character to.

				// Try to add a new row at the bottom of the texture
				if (page.NextRowVerticalOffset + characterHeight + 2.f * padding > textureHeight) {
					// The texture has no more space for the new character to be added

					float maxTextureSize = (float)Texture2D::GetMaximumSize();
					float textureSize = texture->getHeight();	// Texture is always square

					float newTextureSize = textureSize * 2.f;
					if (maxTextureSize >= newTextureSize) {
						// Make the texture twice as bigger
						Image oldTextureImage;
						texture->loadToImage(oldTextureImage);

						Image newTextureImage;
						newTextureImage.create((unsigned)newTextureSize, (unsigned)newTextureSize, glm::uvec4(255, 255, 255, 0));

						texture = Texture2D::Create();
						texture->loadFromImage(newTextureImage);
						texture->update(oldTextureImage);

						addCharacterToTexture(page, character, outlineThickness);
					}
					else {
						// New texture will exceed maximum size
						return false;
					}
				}
				else {
					// Add the character to the next (last added row) row of the texture

					// Make a new row
					Row row;

					// Set row the values based on the last added row
					const Row& lastRow = rows.back();

					row.VerticalOffset = page.NextRowVerticalOffset;
					row.Height = characterHeight + 2.f * padding;
					row.LastHorizontalOffset = characterWidth + 2.f * padding;

					// Set the character offset position
					float newRowVerticalOffset = lastRow.VerticalOffset + lastRow.Height;
					characterOffset.x = padding;
					characterOffset.y = newRowVerticalOffset + padding;
					character.CharacterRectOrigin = characterOffset;

					// And add the row to the row vector.
					rows.push_back(row);

					// Update the vertical offset of the next row
					page.NextRowVerticalOffset += row.Height;

					texture->update(mPixelBuffer, (unsigned)characterWidth, (unsigned)characterHeight, (unsigned)padding, (unsigned)(row.VerticalOffset + padding));
				}
			}
		}

		return true;
	}

	bool Font::setCharSize(unsigned fontSize) const
	{
		// Get current font size
		FT_UShort currentFontSize = mFontFace->size->metrics.x_ppem;

		if (currentFontSize != fontSize) {

			FT_Error result = FT_Set_Pixel_Sizes(mFontFace, 0, fontSize);
			if (result == FT_Err_Invalid_Pixel_Size) {
				ZR_CORE_ERROR("[FONT] Invalid pixel size.");
				ZR_CORE_WARN("{0}", "Failed to set bitmap font size to " + fontSize);
				return false;
			}

			currentFontSize = mFontFace->size->metrics.x_ppem;
			return result == FT_Err_Ok;
		}
		else {
			return true;
		}
	}

	unsigned long long Font::generateCharacterKey(unsigned codePoint, unsigned outlineThickness) const
	{
		return (static_cast<unsigned long long>(outlineThickness) << 32) + codePoint;
	}

	float Font::getKerning(unsigned first, unsigned second, unsigned fontSize) const
	{
		if (first == 0 || second == 0) {
			return 0.f;
		}

		if (mFontFace && FT_HAS_KERNING(mFontFace) && setCharSize(fontSize)) {
			// Convert the characters to indices
			FT_UInt index1 = FT_Get_Char_Index(mFontFace, first);
			FT_UInt index2 = FT_Get_Char_Index(mFontFace, second);

			FT_Vector kerningVector;
			FT_Get_Kerning(mFontFace, index1, index2, FT_KERNING_DEFAULT, &kerningVector);

			return static_cast<float>(kerningVector.x >> 6);
		}
		return 0.f;
	}

	float Font::getLineSpacing(unsigned fontSize) const
	{
		if (mFontFace && setCharSize(fontSize)) {
			return static_cast<float>(mFontFace->size->metrics.height >> 6);
		}
		return 0.0f;
	}
}
