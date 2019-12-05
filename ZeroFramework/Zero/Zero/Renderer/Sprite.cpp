#include <zr_pch.h>

#include "Sprite.h"
#include "RenderCommand.h"
#include "../Core/Log.h"
#include "../Core/Core.h"

namespace zr
{
	const std::string Sprite::sVertexShader = R"(
		#version 330 core

		layout(location = 0) in vec2 aPosition;
		layout(location = 1) in vec2 aTextureCoordinates;

		uniform mat4 uViewProjectionMatrix;
		out vec2 TextureCoordinates;

		void main()
		{
			gl_Position = uViewProjectionMatrix * vec4(aPosition, 0.f, 1.f);
			TextureCoordinates = aTextureCoordinates;
		}
	)";

	const std::string Sprite::sFragmentShader = R"(
		#version 330 core

		out vec4 FragColor;

		uniform sampler2D uSpriteTexture;
		uniform int uDrawAsTexture;
		uniform vec4 uColor;

		in vec2 TextureCoordinates;

		void main()
		{
			if (uDrawAsTexture == 0) {
				FragColor = uColor;
			}
			else {
				FragColor = texture(uSpriteTexture, TextureCoordinates);
			}
		}
	)";

	Sprite::Sprite(bool loadDefaultShader) :
		mShader(),
		mTexture(nullptr),
		mColor(1.f, 1.f, 1.f, 1.f),
		mSpriteVAO(nullptr),
		mTexCoordinates(),
		mVertices(16),
		mNeedsUpdate(false),
		mViewportHeight(0.f)
	{
		mSpriteVAO = VertexArray::Create();

		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, 16 * sizeof(float), DrawMode::Dynamic);
		vbo->setLayout({
			{ShaderDataType::Float2, "aPosition"},
			{ShaderDataType::Float2, "aTextureCoordinates"}
			});
		mSpriteVAO->addVertexBuffer(vbo);

		unsigned indices[]{
			0, 1, 2,
			0, 2, 3
		};

		Ref<IndexBuffer> ebo = IndexBuffer::Create(indices, 6U, DrawMode::Static);
		mSpriteVAO->setIndexBuffer(ebo);

		if (loadDefaultShader) {
			mShader = Shader::Create();
			if (!mShader->loadFromStrings("SpriteShader", Sprite::sVertexShader, Sprite::sFragmentShader)) {
				ZR_CORE_ERROR("[SPRITE] Can't create sprite shader.");
			}
		}

		mTexCoordinates = {
			1.f, 1.f,	// Top Right
			0.f, 1.f,	// Origin: Top Left
			0.f, 0.f,	// Bottom Left
			1.f, 0.f	// Bottom right
		};
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::setTexture(const Ref<Texture2D>& texture)
	{
		if (texture != mTexture) {
			mTexture = texture;
		}

		// Update the sprite size to the actual size of the texture if the size of the sprite wasn't set yet.
		if (mSize.x == 0 || mSize.y == 0) {
			mSize.x = mTexture->getWidth();
			mSize.y = mTexture->getHeight();

			mNeedsUpdate = true;
		}

		if (mTexture != nullptr) {
			mDrawingTexture = true;
		}
	}

	void Sprite::render(const glm::mat4& viewProjectionMatrix)
	{
		//mBorder.draw();

		if (mNeedsUpdate) {
			updateVertices();
			mNeedsUpdate = false;
		}

		mShader->bind();
		mShader->setUniform("uDrawAsTexture", mDrawingTexture);
		mShader->setUniform("uViewProjectionMatrix", viewProjectionMatrix);
		//mShader->setUniform("uSpritePosition", mActualPosition);
		if (mDrawingTexture) {
			mTexture->bindOnTextureSlot(0U);
			mShader->setUniform("uSpriteTexture", 0);
		}
		else {
			mShader->setUniform("uColor", mColor);
		}

		// Save and disable depth test: don't perform depth testing so everything is visualized as it is
		// drawn to the buffer. Objects drawn first will be covered by objects drawn later.
		bool isDoingDepthTest = RenderCommand::GetDepthTestState();
		RenderCommand::EnableDepthTest(false);

		// Enable blend mode
		RenderCommand::EnableBlend(true);

		mSpriteVAO->bind();
		RenderCommand::DrawIndexed(mSpriteVAO);

		// Restore depth testing state back
		RenderCommand::EnableDepthTest(isDoingDepthTest);
		// Disable blend
		RenderCommand::EnableBlend(false);
	}

	void Sprite::updateVertices()
	{
		float vertices[]{
			mPosition.x + mSize.x,	mViewportHeight - mPosition.y,				mTexCoordinates[0], mTexCoordinates[1], // Top Right
			mPosition.x,			mViewportHeight - mPosition.y,				mTexCoordinates[2], mTexCoordinates[3], // Origin: Top Left
			mPosition.x,			mViewportHeight - mPosition.y - mSize.y,		mTexCoordinates[4], mTexCoordinates[5],	// Bottom Left
			mPosition.x + mSize.x,  mViewportHeight - mPosition.y - mSize.y,		mTexCoordinates[6], mTexCoordinates[7]	// Bottom Right
		};

		std::memcpy(&mVertices[0], vertices, 16 * sizeof(float));

		mSpriteVAO->bind();
		mSpriteVAO->getVertexBuffers()[0]->setData(&mVertices[0], static_cast<unsigned>(mVertices.size() * sizeof(float)));
	}

	void Sprite::onPositionUpdate()
	{
		mNeedsUpdate = true;
		//mBorder.setPosition(mPosition);
	}

	void Sprite::onSizeUpdate()
	{
		mNeedsUpdate = true;
		//mBorder.setSize(mSize);
	}

	void Sprite::onViewportUpdate(const glm::vec2& viewportSize)
	{
		mNeedsUpdate = true;
		ZR_INFO("[Sprite] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		mViewportHeight = viewportSize.y;
		//mBorder.viewportUpdate(mViewportSize);
	}

	void Sprite::setRect(const glm::uvec4& rect)
	{
		setRect(rect.x, rect.y, rect.z, rect.w);
	}

	void Sprite::setRect(int xPosition, int yPosition, unsigned width, unsigned height)
	{
		mPosition.x = (float)xPosition;
		mPosition.y = (float)yPosition;
		mSize.x = (float)width;
		mSize.y = (float)height;

		//mBorder.setPosition(mPosition);
		//mBorder.setSize(mSize);

		// Update vertices
		mNeedsUpdate = true;
	}

	void Sprite::setRect(const glm::uvec2& position, const glm::uvec2& size)
	{
		setRect(position.x, position.y, size.x, size.y);
	}

	void Sprite::setTextureCoordinates(unsigned texPositionX, unsigned texPositionY, unsigned width, unsigned height)
	{
		if (mTexture != nullptr) {
			float textureWidth = mTexture->getWidth();;
			float textureHeight = mTexture->getHeight();

			// These coordinates are switched vertically in order to get the correct texture coordinates out of the
			// sprite atlas.
			mTexCoordinates[0] = static_cast<float>(texPositionX + width - 1U) / textureWidth; // Bottom Right X
			mTexCoordinates[1] = static_cast<float>(textureHeight - (texPositionY)) / textureHeight;  // Bottom Right Y
			mTexCoordinates[2] = static_cast<float>(texPositionX) / textureWidth; // Bottom Left X
			mTexCoordinates[3] = static_cast<float>(textureHeight - (texPositionY)) / textureHeight; // Bottom Left Y

			mTexCoordinates[4] = static_cast<float>(texPositionX) / textureWidth;					// Top Left X
			mTexCoordinates[5] = static_cast<float>(textureHeight - (texPositionY + height - 1U)) / textureHeight;	// Top Left Y

			mTexCoordinates[6] = static_cast<float>(texPositionX + width - 1U) / textureWidth;		// Top Right X
			mTexCoordinates[7] = static_cast<float>(textureHeight - (texPositionY + height - 1U)) / textureHeight;	// Top Right Y

			mNeedsUpdate = true;
		}
	}

	const glm::vec4 Sprite::getRect() const
	{
		return glm::vec4(mPosition, mSize);
	}

	void Sprite::setColor(const glm::vec4& color)
	{
		mColor = color;
		mDrawingTexture = false;
	}

	void Sprite::setColorOpacity(float opacity)
	{
		mColor.a = clamp(0.f, 1.f, opacity);
	}

	void Sprite::setBorderThickness(unsigned borderThickness)
	{
		//mBorder.setThickness(borderThickness);
	}

	void Sprite::setBorderColor(const glm::vec4& borderColor)
	{
		//mBorder.setColor(borderColor);
	}
}
