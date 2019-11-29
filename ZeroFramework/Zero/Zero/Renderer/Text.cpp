#include <zr_pch.h>

#include "RenderCommand.h"
#include "Text.h"

#include "Renderer2D.h"

#include "../Core/Log.h"

namespace zr
{
	Text::Text() :
		Movable(),
		mFont(nullptr),
		mFontSize(30U),
		mOutlineThickness(0U),
		mString(),
		mColor(1.f, 1.f, 1.f, 1.f),
		mOutlineColor(0.f, 0.f, 0.f, 1.f),
		mQuadSize(0U, 0U)
	{
	}

	Text::Text(const std::string& string, const std::shared_ptr<Font>& font, unsigned fontSize, unsigned outlineThickness) :
		Movable(),
		mFont(font),
		mFontSize(fontSize),
		mOutlineThickness(outlineThickness),
		mString(string),
		mColor(1.f, 1.f, 1.f, 1.f),
		mOutlineColor(0.f, 0.f, 0.f, 1.f),
		mQuadSize(0U, 0U)
	{
		if (mFont != nullptr && !string.empty()) {
			dryRun();
		}
	}

	Text::~Text()
	{
	}

	void Text::setFont(const std::shared_ptr<Font>& font)
	{
		if (!&font) {
			return;
		}

		if (mFont != font) {
			mFont = font;
			dryRun();
		}
	}

	void Text::setString(const std::string& string)
	{
		if (mString != string) {
			mString = string;

			dryRun();
		}
	}

	void Text::setNumber(unsigned value)
	{
		setString(std::to_string(value));
	}

	void Text::setFontSize(unsigned fontSize)
	{
		if (mFontSize != fontSize) {
			mFontSize = fontSize;
			if (mFont != nullptr) {
				dryRun();
			}
		}
	}

	void Text::setFillColor(const glm::vec4& color)
	{
		mColor.r = clamp(0.f, 1.f, color.r);
		mColor.g = clamp(0.f, 1.f, color.g);
		mColor.b = clamp(0.f, 1.f, color.b);
		mColor.a = clamp(0.f, 1.f, color.a);
	}

	void Text::setOutlineThickness(unsigned outlineThickness)
	{
		if (outlineThickness >= 0U && outlineThickness != mOutlineThickness) {
			mOutlineThickness = outlineThickness;
			dryRun();
		}
	}

	void Text::setOutlineColor(const glm::vec4& color)
	{
		mOutlineColor.r = clamp(0.f, 1.f, color.r);
		mOutlineColor.g = clamp(0.f, 1.f, color.g);
		mOutlineColor.b = clamp(0.f, 1.f, color.b);
		mOutlineColor.a = clamp(0.f, 1.f, color.a);
	}

	const glm::vec2& Text::getSize() const
	{
		return mQuadSize;
	}

	void Text::render(const glm::mat4& viewProjectionMatrix)
	{
		if (!mString.empty()) {
			const Ref<Texture2D>& fontTexture = mFont->getTexture(mFontSize);

			float textureWidth = static_cast<float>(fontTexture->getWidth());
			float textureHeight = static_cast<float>(fontTexture->getHeight());

			unsigned lastChar = 0U;
			float outlineXPosition = mPosition.x;
			float outlineYPosition = mViewportHeight - (mPosition.y + mMaxVerticalBearing);

			float fillXPosition = outlineXPosition + mOutlineThickness;
			float fillYPosition = mViewportHeight - (mPosition.y + mMaxVerticalBearing + mOutlineThickness);

			for (char c : mString) {
				if (c == ' ') {
					unsigned adv = mFont->getCharacter(c, mFontSize).Advance;
					outlineXPosition += adv;
					fillXPosition += adv;
					lastChar = c;
					continue;
				}

				switch (c) {
					case '\n':
					{
						outlineXPosition = mPosition.x;
						fillXPosition = outlineXPosition + mOutlineThickness;
						outlineYPosition -= mFont->getLineSpacing(mFontSize);
						fillYPosition -= mFont->getLineSpacing(mFontSize);
						lastChar = 0;
						continue;
						break;
					}
					case '\t':
					{
						unsigned spaceWidth = mFont->getCharacter(' ', mFontSize, mOutlineThickness).Advance;
						outlineXPosition += spaceWidth * 4;
						fillXPosition += spaceWidth * 4;
						lastChar = ' ';
						break;
					}
					default:
					break;
				}

				float kerning = mFont->getKerning(lastChar, c, mFontSize);
				lastChar = c;

				if (mOutlineThickness != 0U) {
					// Draw fill character vertices
					const Font::Character& outlineCharacter = mFont->getCharacter(c, mFontSize, mOutlineThickness);
					drawCharacter(outlineCharacter, outlineXPosition, outlineYPosition, kerning, textureWidth, textureHeight, fontTexture->getHandle(), mOutlineColor);
					// Advances to the next character position
					outlineXPosition += outlineCharacter.Advance;
				}

				// Draw fill character vertices
				const Font::Character& fillCharacter = mFont->getCharacter(c, mFontSize);
				drawCharacter(fillCharacter, fillXPosition, fillYPosition, kerning, textureWidth, textureHeight, fontTexture->getHandle(), mColor);
				// Advances to the next character position
				fillXPosition += fillCharacter.Advance;
			}
		}
		// Empty string: draw nothing
	}

	void Text::dryRun()
	{
		if (mFont != nullptr) {
			mQuadSize.x = 0U;
			mQuadSize.y = 0U;
			float lineWidth = 0.f;
			unsigned lastChar = 0U;
			int lastCharAdvance = 0;
			mMaxVerticalBearing = 0U;

			for (char c : mString) {
				const Font::Character& ch = mFont->getCharacter(c, mFontSize, mOutlineThickness);
				mQuadSize.y = std::max<unsigned>(mQuadSize.y, ch.Size.y);

				switch (c) {
					case '\n':
					{
						lineWidth = 0;
						mQuadSize.y += mFont->getLineSpacing(mFontSize);
						lastChar = 0;
						continue;
					}
					case '\t':
					{
						lineWidth += mFont->getCharacter(' ', mFontSize, mOutlineThickness).Advance * 4U;
						lastChar = ' ';
						break;
					}
					default:
					break;
				}

				float kerning = mFont->getKerning(lastChar, c, mFontSize);
				lastCharAdvance = ch.Advance - ch.Size.x;

				if (lastChar == 0U && ch.Bearing.x != 0) {
					lineWidth += ch.Advance + ch.Bearing.x;
				}
				else {
					lineWidth += ch.Advance;
				}

				mMaxVerticalBearing = std::max<float>(mMaxVerticalBearing, ch.Bearing.y);
				mQuadSize.x = std::max<unsigned>(mQuadSize.x, lineWidth);
				lastChar = c;
			}
			mQuadSize.x -= lastCharAdvance;
			onSizeUpdate();
			mNeedsUpdate = true;
		}
	}

	void Text::drawCharacter(const Font::Character& character, float characterXPosition, float characterYPosition, float kerning, float textureWidth, float textureHeight, unsigned textureHandle, const glm::vec4& color) const
	{
		float charWidth = character.Size.x;
		float charHeight = character.Size.y;

		float xPos = characterXPosition + character.Bearing.x + kerning;
		float yPos = characterYPosition - charHeight + character.Bearing.y;

		float topLeftX = character.CharacterRectOrigin.x;		// Top left x
		float topLeftY = character.CharacterRectOrigin.y;		// Top left y
		float topRightX = topLeftX + charWidth;				// Top right x
		float topRightY = topLeftY;							// Top right y
		float bottomLeftX = topLeftX;						// Bottom left x
		float bottomLeftY = topLeftY + charHeight;			// Bottom left y
		float bottomRightX = topLeftX + charWidth;			// Bottom right x
		float bottomRightY = topLeftY + charHeight;			// Bottom right y

		float tlxTexCoord = topLeftX / textureWidth;		// Top left x
		float tlyTexCoord = topLeftY / textureHeight;		// Top left y
		float trxTexCoord = topRightX / textureWidth;		// Top right x
		float tryTexCoord = topRightY / textureHeight;		// Top right y
		float blxTexCoord = bottomLeftX / textureWidth;		// Bottom left x
		float blyTexCoord = bottomLeftY / textureHeight;	// Bottom left y
		float brxTexCoord = bottomRightX / textureWidth;	// Bottom right x
		float bryTexCoord = bottomRightY / textureHeight;	// Top right

		std::vector<glm::vec3> positions{
			{xPos + charWidth, yPos, 0.f},
			{xPos, yPos, 0.f},
			{xPos, yPos + charHeight, 0.f},
			{xPos + charWidth, yPos + charHeight, 0.f}
		};

		std::vector<glm::vec2> textureCoordinates{
			{brxTexCoord, bryTexCoord},
			{blxTexCoord, blyTexCoord},
			{tlxTexCoord, tlyTexCoord},
			{trxTexCoord, tryTexCoord}
		};

		Renderer2D::DrawQuad(positions, textureCoordinates, { 0, 1, 2, 0, 2, 3 }, textureHandle, color);
	}

	void Text::onViewportUpdate(const glm::vec2& viewportSize)
	{
		//updateProjectionMatrix();
		ZR_CORE_INFO("[TEXT] Viewport updated to: {0}, {1}", viewportSize.x, viewportSize.y);
		mViewportHeight = viewportSize.y;
		dryRun();
	}

	void Text::onPositionUpdate()
	{
		//mViewportHeight = ViewportDependable::GetViewportSize().y - 2.f * mPosition.y;
	}

	void Text::onSizeUpdate()
	{
	}
}
