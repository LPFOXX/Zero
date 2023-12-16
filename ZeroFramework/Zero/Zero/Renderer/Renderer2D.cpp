#include <zr_pch.h>

#include "Renderer2D.h"
#include "RenderCommand.h"

#include "SFML/System.hpp"

#define USES_BATCHES 1
#define COMPUTE_MATRIX_TRANSFORM 1

namespace zr
{
	void Renderer2D::Init()
	{
		ZR_PROFILER_FUNCTION();

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
		ZR_PROFILER_FUNCTION();

		delete sData;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		ZR_PROFILER_FUNCTION();

		sData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uViewProjection", sData->ViewProjectionMatrix);
		sData->ViewMatrix = camera->getViewMatrix();
		sData->Framebuffer = nullptr;
	}

	void Renderer2D::BeginScene(const Ref<Viewer>& viewer)
	{
		ZR_PROFILER_FUNCTION();

		const auto& camera = viewer->getCamera();
		sData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uViewProjection", sData->ViewProjectionMatrix);
		sData->ViewMatrix = camera->getViewMatrix();
		sData->Framebuffer = viewer->getFramebuffer();
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera, const Ref<Framebuffer>& framebuffer)
	{
		ZR_PROFILER_FUNCTION();

		sData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uViewProjection", sData->ViewProjectionMatrix);
		sData->ViewMatrix = camera->getViewMatrix();
		sData->Framebuffer = framebuffer;
	}

	void Renderer2D::EndScene()
	{
		ZR_PROFILER_FUNCTION();

		sData->BatchManager->flush(sData->ViewProjectionMatrix);

		if (sData->Framebuffer != nullptr) {
			sData->Framebuffer->blit();
			Framebuffer::BindDefault();
		}
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.f }, size, angle, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		#if COMPUTE_MATRIX_TRANSFORM
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), angle, { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		Renderer2D::DrawQuad(transform, color);
		#else

		const glm::vec4& left = sData->ViewMatrix[0];
		const glm::vec4& up = sData->ViewMatrix[1];

		glm::vec2 newSize = size * .5f;

		std::vector<glm::vec3> pos{
			{position.x + (-left.x - up.x) * newSize.x, position.y + (-left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x - up.x) * newSize.x, position.y + (left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x + up.x) * newSize.x, position.y + (left.y + up.y) * newSize.y, position.z},
			{position.x + (-left.x + up.x) * newSize.x, position.y + (-left.y + up.y) * newSize.y, position.z},
		};

		Renderer2D::DrawQuad(pos, { color }, { 0, 1, 2, 2, 3, 0 });
		#endif
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		#if COMPUTE_MATRIX_TRANSFORM
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		Renderer2D::DrawQuad(transform, color);
		#else

		const glm::vec4& left = sData->ViewMatrix[0];
		const glm::vec4& up = sData->ViewMatrix[1];

		glm::vec2 newSize = size * .5f;

		std::vector<glm::vec3> pos{
			{position.x + (-left.x - up.x) * newSize.x, position.y + (-left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x - up.x) * newSize.x, position.y + (left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x + up.x) * newSize.x, position.y + (left.y + up.y) * newSize.y, position.z},
			{position.x + (-left.x + up.x) * newSize.x, position.y + (-left.y + up.y) * newSize.y, position.z},
		};

		Renderer2D::DrawQuad(pos, { color }, { 0, 1, 2, 2, 3, 0 });
		#endif
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		std::vector<glm::vec3> positions{
			transform * glm::vec4(-.5f, -.5f, .0f, 1.f),
			transform * glm::vec4(.5f, -.5f, .0f, 1.f),
			transform * glm::vec4(.5f,  .5f, .0f, 1.f),
			transform * glm::vec4(-.5f,  .5f, .0f, 1.f)
		};

		Renderer2D::DrawQuad(positions, { color }, { 0, 1, 2, 2, 3, 0 });
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4& tintingColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.f }, size, angle, texture, textureScalingFactor, tintingColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4& tintingColor)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, textureScalingFactor, tintingColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4& tintingColor)
	{
		#if COMPUTE_MATRIX_TRANSFORM
		#if USES_BATCHES
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), angle, { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		Renderer2D::DrawQuad(transform, texture, textureScalingFactor, tintingColor);
		#else
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uTextureScalingFactor", textureScalingFactor);
		//sData->WhiteTexture->bind();
		sData->TextureShader->setUniform("uTintingColor", tintingColor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), angle, { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		sData->TextureShader->setUniform("uTransform", transform);

		texture->bind();

		sData->QuadVertexArray->bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
		#endif
		#else
		const glm::vec4& left(sData->ViewMatrix[0]);
		const glm::vec4& up(sData->ViewMatrix[1]);

		glm::vec2 newSize = size * .5f;

		std::vector<glm::vec3> pos{
			{position.x + (-left.x - up.x) * newSize.x, position.y + (-left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x - up.x) * newSize.x, position.y + (left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x + up.x) * newSize.x, position.y + (left.y + up.y) * newSize.y, position.z},
			{position.x + (-left.x + up.x) * newSize.x, position.y + (-left.y + up.y) * newSize.y, position.z},
		};

		std::vector<glm::vec2> textureCoordinates{
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
			{0.f, 1.f},
		};
		Renderer2D::DrawQuad(pos, textureCoordinates, { 0, 1, 2, 2, 3, 0 }, texture->getHandle(), tintingColor, textureScalingFactor);
		#endif
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4& tintingColor)
	{
		#if COMPUTE_MATRIX_TRANSFORM
		#if USES_BATCHES
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		Renderer2D::DrawQuad(transform, texture, textureScalingFactor, tintingColor);
		#else
		sData->TextureShader->bind();
		sData->TextureShader->setUniform("uTextureScalingFactor", textureScalingFactor);
		//sData->WhiteTexture->bind();
		sData->TextureShader->setUniform("uTintingColor", tintingColor);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		sData->TextureShader->setUniform("uTransform", transform);

		texture->bind();

		sData->QuadVertexArray->bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
		#endif
		#else
		const glm::vec4& left(sData->ViewMatrix[0]);
		const glm::vec4& up(sData->ViewMatrix[1]);

		glm::vec2 newSize = size * .5f;

		std::vector<glm::vec3> pos{
			{position.x + (-left.x - up.x) * newSize.x, position.y + (-left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x - up.x) * newSize.x, position.y + (left.y - up.y) * newSize.y, position.z},
			{position.x + (left.x + up.x) * newSize.x, position.y + (left.y + up.y) * newSize.y, position.z},
			{position.x + (-left.x + up.x) * newSize.x, position.y + (-left.y + up.y) * newSize.y, position.z},
		};

		std::vector<glm::vec2> textureCoordinates{
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
			{0.f, 1.f},
		};
		Renderer2D::DrawQuad(pos, textureCoordinates, { 0, 1, 2, 2, 3, 0 }, texture->getHandle(), tintingColor, textureScalingFactor);
		#endif
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor, const glm::vec4& tintingColor)
	{
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

		Renderer2D::DrawQuad(positions, textureCoordinates, { 0, 1, 2, 2, 3, 0 }, texture->getHandle(), tintingColor, textureScalingFactor);
	}

	void Renderer2D::DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCoordinates, const std::vector<unsigned>& indices, unsigned textureId, const glm::vec4& color, const glm::vec2& scalingFactor)
	{
		if (positions.size() != 4 || textureCoordinates.size() != 4 || indices.size() != 6) {
			return;
		}

		std::vector<BatchVertexTypes::ExtendedVertex> vertices;
		for (unsigned i = 0; i < 4; ++i) {
			vertices.emplace_back(positions[i], textureCoordinates[i]);
		}
		sData->BatchManager->addVertices(vertices, indices, textureId, color, scalingFactor);
	}

	void Renderer2D::DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors, const std::vector<unsigned>& indices)
	{
		if (positions.size() != 4 || indices.size() != 6) {
			return;
		}

		std::vector<glm::vec4> verticesColors;
		glm::vec4 defaultColor(1.f, 1.f, 1.f, 1.f);
		unsigned colorCount = colors.size();
		if (colorCount == 0) {
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
			verticesColors.push_back(defaultColor);
		}
		else if (colorCount == 1) {
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[0]);
		}
		else if (colorCount == 2) {
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[1]);
			verticesColors.push_back(colors[0]);
			verticesColors.push_back(colors[1]);
		}
		else if (colorCount == 3) {
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

	void Renderer2D::DrawShape(const Ref<Shape> shape, const glm::vec2& position, float scale)
	{
		DrawShape(shape, { position.x, position.y, 0.f }, scale);
	}

	void Renderer2D::DrawShape(const Ref<Shape> shape, const glm::vec3& position, float scale)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { scale, scale, 1.f });
		DrawShape(shape, transform);
	}

	void Renderer2D::DrawRotatedShape(const Ref<Shape>& shape, const glm::vec2& position, float scale, float angle)
	{
		Renderer2D::DrawRotatedShape(shape, { position.x, position.y, 0.f }, scale, angle);
	}

	void Renderer2D::DrawRotatedShape(const Ref<Shape>& shape, const glm::vec3& position, float scale, float angle)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), angle, { 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), { scale, scale, 1.f });
		DrawShape(shape, transform);
	}

	void Renderer2D::Draw(const Drawable2D& drawable)
	{
		const auto& objectVertices = drawable.getVertices();
		const auto& objectIndices = drawable.getIndices();
		const auto& colors = drawable.getColors();

		std::vector<BatchVertexTypes::ColoredVertex> innerVertices;
		for (unsigned i = 0; i < objectVertices.size(); ++i) {
			innerVertices.emplace_back(objectVertices[i], colors[i]);
		}

		sData->BatchManager->addVertices(innerVertices, objectIndices);
	}

	void Renderer2D::DrawShape(const Ref<Shape>& shape, const glm::mat4& transform)
	{
		const auto& shapeVertices = shape->getVertices();
		const auto& shapeOutline = shape->getOutline();

		std::vector<BatchVertexTypes::ColoredVertex> innerVertices;
		std::transform(shapeVertices.begin(), shapeVertices.end(), std::back_inserter(innerVertices), [&transform, &shape](const glm::vec3& vertex) {
			return BatchVertexTypes::ColoredVertex(transform * glm::vec4(vertex, 1.f), shape->getFillColor());
		});
		sData->BatchManager->addVertices(innerVertices, shape->getIndices(), shape->getPrimitiveType());

		if (shapeOutline.getThickness() > 0.f) {
			std::vector<BatchVertexTypes::ColoredVertex> outlineVertices;
			const auto& shapeOutlineVertices = shapeOutline.getVertices();
			std::transform(shapeOutlineVertices.begin(), shapeOutlineVertices.end(), std::back_inserter(outlineVertices), [&transform, &shapeOutline](const glm::vec3& vertex) {
				return BatchVertexTypes::ColoredVertex(transform * glm::vec4(vertex, 1.f), shapeOutline.getColor());
			});
			sData->BatchManager->addVertices(outlineVertices, shapeOutline.getIndices(), shapeOutline.getPrimitiveType());
		}
	}
}
