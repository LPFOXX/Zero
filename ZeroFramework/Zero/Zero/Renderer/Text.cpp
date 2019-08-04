#include <zr_pch.h>

#include "RenderCommand.h"
#include "Text.h"

#include "../Log.h"

namespace zr
{
	std::string Text::sVertexShader = R"(
		#version 330 core

		layout (location = 0) in vec2 aVertexPos;
		layout (location = 1) in vec2 aVertexTexCoord;

		out vec2 TexCoords;
		uniform mat4 uViewProjection;
		uniform vec2 uTextPosition;

		void main()
		{
			gl_Position = uViewProjection * vec4(aVertexPos + uTextPosition, 0.0, 1.0);
			TexCoords = aVertexTexCoord;
		}
	)";

	std::string Text::sFragmentShader = R"(
		#version 330 core

		in vec2 TexCoords;
		out vec4 Color;

		uniform sampler2D uText;
		uniform vec4 uTextColor;

		void main()
		{
			Color = uTextColor * vec4(1.0, 1.0, 1.0, texture(uText, TexCoords).a);
		}
	)";

	bool Text::sIsInitialized = false;
	std::shared_ptr<VertexArray> Text::sVAO = nullptr;
	std::shared_ptr<Shader> Text::sShader = nullptr;

	unsigned Text::sReferenceCount = 0U;

	Text::Text() :
		Movable(),
		mFont(nullptr),
		mFontSize(30U),
		mOutlineThickness(0U),
		mString(),
		mColor(255.f, 255.f, 255.f, 255.f),
		mQuadSize(0U, 0U)
	{
		//updateProjectionMatrix();
		initializeStaticObjects();
		Text::sReferenceCount++;
	}

	Text::Text(const std::string& string, const std::shared_ptr<Font>& font, unsigned fontSize, unsigned outlineThickness) :
		Movable(),
		mFont(font),
		mFontSize(fontSize),
		mOutlineThickness(outlineThickness),
		mString(string),
		mColor({ 255.f, 255.f, 255.f, 255.f }),
		mQuadSize(0U, 0U)
	{
		if (mFont != nullptr && !string.empty()) {
			dryRun();
		}

		//updateProjectionMatrix();
		initializeStaticObjects();
		Text::sReferenceCount++;
	}

	Text::~Text()
	{
		if (Text::sReferenceCount == 1U) {
			Text::sReferenceCount = 0U;
			Text::sVAO.reset();
			Text::sShader.reset();
			Text::sIsInitialized = false;
			return;
		}

		Text::sReferenceCount--;
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
		if (color.r < 0.f || color.g < 0.f || color.b < 0.f) {
			return;
		}

		mColor.r = color.r >= 1.f ? 1.f : color.r;
		mColor.g = color.g >= 1.f ? 1.f : color.g;
		mColor.b = color.b >= 1.f ? 1.f : color.b;
		mColor.a = color.a >= 1.f ? 1.f : color.a;
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
		mOutlineColor = color;
	}

	const glm::uvec2& Text::getSize() const
	{
		return mQuadSize;
	}

	void Text::render(const glm::mat4& viewProjectionMatrix)
	{
		if (!mString.empty()) {
			const std::shared_ptr<Texture>& fontTexture = mFont->getTexture(mFontSize);
			// Bind the buffer
			Text::sVAO->bind();

			// Activate blend mode
			RenderCommand::EnableBlend(true);

			Text::sShader->bind();
			Text::sShader->setUniform("uText", 0);
			fontTexture->bindOnTextureUnit(0);
			Text::sShader->setUniform("uViewProjection", viewProjectionMatrix);

			float textureWidth = static_cast<float>(fontTexture->getWidth());
			float textureHeight = static_cast<float>(fontTexture->getHeight());

			unsigned lastChar = 0U;
			float outlineXPosition = mPosition.x;
			float outlineYPosition = (mPosition.y - mMaxVerticalBearing);

			float fillXPosition = outlineXPosition + mOutlineThickness;
			float fillYPosition = (mPosition.y - mMaxVerticalBearing - mOutlineThickness);

			// Save depth mask state
			// Disable test mask
			// Draw
			// Reset depth mask state
			bool isDoingDepthTest = RenderCommand::GetDepthTestState();
			RenderCommand::EnableDepthTest(false);


			mOutlineVertices.clear();
			mOutlineVertices.reserve(16 * mString.size());
			mFillVertices.clear();
			mFillVertices.reserve(16 * mString.size());

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
					const Font::Character& outline = mFont->getCharacter(c, mFontSize, mOutlineThickness);

					float charWidth = outline.Size.x;
					float charHeight = outline.Size.y;

					float xPos = outlineXPosition + outline.Bearing.x + kerning;
					float yPos = outlineYPosition - charHeight + outline.Bearing.y;

					// Advances to the next character position
					outlineXPosition += outline.Advance;

					float topLeftX = outline.CharacterRectOrigin.x;		// Top left x
					float topLeftY = outline.CharacterRectOrigin.y;		// Top left y
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

					// Bottom and top coordinates are actually flipped
					// because of the system coordinates of textures.
					float vertices[]{
						xPos + charWidth,	yPos,					brxTexCoord, bryTexCoord,	// Bottom right
						xPos,				yPos,					blxTexCoord, blyTexCoord,	// Bottom left
						xPos,				yPos + charHeight,		tlxTexCoord, tlyTexCoord,	// Top left
						xPos + charWidth,	yPos + charHeight,		trxTexCoord, tryTexCoord 	// Top right 
					};

					mOutlineVertices.insert(mOutlineVertices.end(), vertices, vertices + 16);

					Text::sShader->setUniform("uTextColor", mOutlineColor);
					Text::sShader->setUniform("uTextPosition", mActualPosition);

					// Update the buffer
					//Text::sVBOupdateData(vertices, sizeof(vertices));
					Text::sVAO->getVertexBuffers()[0]->bind();
					Text::sVAO->getVertexBuffers()[0]->setData(vertices, sizeof(vertices));

					RenderCommand::DrawIndexed(Text::sVAO);
				}

				// Compute fill vertices
				const Font::Character& fill = mFont->getCharacter(c, mFontSize);

				float charWidth = fill.Size.x;
				float charHeight = fill.Size.y;

				float xPos = fillXPosition + fill.Bearing.x + kerning;
				float yPos = fillYPosition - charHeight + fill.Bearing.y;

				fillXPosition += fill.Advance;

				float topLeftX = fill.CharacterRectOrigin.x;		// Top left x
				float topLeftY = fill.CharacterRectOrigin.y;		// Top left y
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

				// Bottom and top coordinates are actually flipped
				// because of the system coordinates of textures.
				float vertices[]{
					xPos + charWidth,	(yPos),					brxTexCoord, bryTexCoord,	// Bottom right
					xPos,				(yPos),					blxTexCoord, blyTexCoord,	// Bottom left
					xPos,				(yPos + charHeight),	tlxTexCoord, tlyTexCoord,	// Top left
					xPos + charWidth,	(yPos + charHeight),	trxTexCoord, tryTexCoord 	// Top right 
				};

				mFillVertices.insert(mFillVertices.end(), vertices, vertices + 16);

				Text::sShader->setUniform("uTextColor", mColor);
				Text::sShader->setUniform("uTextPosition", mActualPosition);
				// Update the buffer
				Text::sVAO->getVertexBuffers()[0]->setData(vertices, sizeof(vertices));
				RenderCommand::DrawIndexed(Text::sVAO);
			}



			// Restore depth testing state
			RenderCommand::EnableDepthTest(isDoingDepthTest);
			RenderCommand::EnableBlend(false);
		}
		// Empty string: draw nothing
	}

	void Text::dryRun()
	{
		if (mFont != nullptr) {
			mQuadSize.x = 0U;
			mQuadSize.y = 0U;
			unsigned lineWidth = 0U;
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

	void Text::onViewportUpdate(const glm::vec2& viewportSize)
	{
		//updateProjectionMatrix();
		ZR_CORE_INFO("[TEXT] Viewport updated to: {0}, {1}", viewportSize.x, viewportSize.y);
		mActualPosition.y = viewportSize.y - 2.f * mPosition.y;
		dryRun();
	}


	void Text::initializeStaticObjects()
	{
		if (Text::sReferenceCount == 0U && !Text::sIsInitialized) {
			unsigned indices[]{
				0, 1, 2,
				0, 2, 3
			};

			Text::sVAO.reset(VertexArray::Create());
			std::shared_ptr<VertexBuffer> vbo;
			vbo.reset(VertexBuffer::Create(nullptr, 16 * sizeof(float), DrawMode::Dynamic));
			vbo->setLayout({
				{ ShaderDataType::Float2, "aVertexPos"},
				{ ShaderDataType::Float2, "aVertexTexCoord"}
				});

			Text::sVAO->addVertexBuffer(vbo);

			std::shared_ptr<IndexBuffer> ebo;
			ebo.reset(IndexBuffer::Create(indices, 6U, DrawMode::Static));
			Text::sVAO->setIndexBuffer(ebo);

			Text::sShader.reset(Shader::Create());
			if (!Text::sShader->loadFromStrings(Text::sVertexShader, Text::sFragmentShader)) {
				ZR_CORE_ERROR("Can't create Text shader.");
			}

			Text::sIsInitialized = true;
		}
	}

	void Text::onPositionUpdate()
	{
		mActualPosition.y = ViewportDependable::GetViewportSize().y - 2.f * mPosition.y;
	}

	void Text::onSizeUpdate()
	{
	}
}
