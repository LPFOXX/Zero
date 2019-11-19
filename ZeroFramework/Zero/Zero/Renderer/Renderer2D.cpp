#include <zr_pch.h>

#include "Renderer2D.h"
#include "RenderCommand.h"

namespace zr
{
	void Renderer2D::Init()
	{
		sData = new Renderer2DStorage;
		sData->BatchManager = std::make_shared<BatchManager>();

		sData->QuadVertexArray = VertexArray::Create(true);

		float squareVertices[5 * 4]{
			-.5f, -.5f, 0.0f,		0.f, 0.f,
			 .5f, -.5f, 0.0f,		1.f, 0.0f,
			 .5f,  .5f, 0.0f,		1.f, 1.f,
			-.5f,  .5f, 0.0f,		0.f, 1.f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices), DrawMode::Static);
		squareVB->setLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float2, "aTextureCoordinates"}
			});
		sData->QuadVertexArray->addVertexBuffer(squareVB);

		unsigned squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, 6, DrawMode::Static);
		sData->QuadVertexArray->setIndexBuffer(squareIB);

		Image whiteImage;
		whiteImage.create(1U, 1U, glm::uvec4(255));

		sData->WhiteTexture = Texture2D::Create();
		sData->WhiteTexture->loadFromImage(whiteImage);

		sData->TextureShader = Shader::Create("Renderer2DTextureShader", "resources/shaders/texture.vs", "resources/shaders/texture.fs");
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uTexture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete sData;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		sData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uViewProjection", sData->ViewProjectionMatrix);
	}

	void Renderer2D::EndScene()
	{
		sData->BatchManager->flush(sData->ViewProjectionMatrix);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, angle, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(angle), { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		std::vector<glm::vec3> positions{
			transform * glm::vec4(-.5f, -.5f, .0f, 1.f),
			transform * glm::vec4( .5f, -.5f, .0f, 1.f),
			transform * glm::vec4( .5f,  .5f, .0f, 1.f),
			transform * glm::vec4(-.5f,  .5f, .0f, 1.f)
		};

		Renderer2D::DrawQuad(positions, { color, color, color, color }, { 0, 1, 2, 2, 3, 0 });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4 & tintingColor)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, angle, texture, textureScalingFactor, tintingColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4 & tintingColor)
	{
		#define USES_BATCHES 0
		#if USES_BATCHES
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(angle), { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		std::vector<glm::vec3> positions{
			transform * glm::vec4(-.5f, -.5f, .0f, 1.f),
			transform * glm::vec4(.5f, -.5f, .0f, 1.f),
			transform * glm::vec4(.5f,  .5f, .0f, 1.f),
			transform * glm::vec4(-.5f,  .5f, .0f, 1.f)
		};

		std::vector<glm::vec2> textureCoordinates{
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
			{0.f, 1.f},
		};
		Renderer2D::DrawQuad(positions, textureCoordinates, { 0, 1, 2, 2, 3, 0 }, texture->getHandle());
		#else
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uTextureScalingFactor", textureScalingFactor);
		//sData->WhiteTexture->bind();
		sData->TextureShader->setUniform("uTintingColor", tintingColor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(angle), { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		sData->TextureShader->setUniform("uTransform", transform);

		texture->bind();

		sData->QuadVertexArray->bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
		#endif
	}

	void Renderer2D::DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCoordinates, const std::vector<unsigned>& indices, unsigned textureId)
	{
		if (positions.size() != 4 || textureCoordinates.size() != 4 || indices.size() != 6) {
			return;
		}

		std::vector<BatchVertexTypes::ExtendedVertex> vertices;
		for (unsigned i = 0; i < 4; ++i) {
			vertices.emplace_back(positions[i], textureCoordinates[i]);
		}
		sData->BatchManager->addVertices(vertices, indices, textureId);
	}

	void Renderer2D::DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors, const std::vector<unsigned>& indices)
	{
		if (positions.size() != 4 || indices.size() != 6) {
			return;
		}

		std::vector<glm::vec4> verticesColors;
		glm::vec4 defaultColor(1.f, 1.f, 1.f, 1.f);
		if (colors.size() == 0) {
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
		}
		else if (colors.size() == 1) {
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
		}
		else if (colors.size() == 2) {
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[1]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[1]);
		}
		else if (colors.size() == 3) {
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[1]);
			verticesColors.push_back(colors[2]);
			verticesColors.push_back(colors[1]);
		}
		else {
			verticesColors = colors;
		}

		std::vector<BatchVertexTypes::ColoredVertex> vertices;
		for (unsigned i = 0; i < 4; ++i) {
			vertices.emplace_back(positions[i], verticesColors[i]);
		}
		sData->BatchManager->addVertices(vertices, indices);
	}
}
