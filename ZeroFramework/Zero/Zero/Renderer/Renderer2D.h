#pragma once

#include "Camera.h"
#include "Buffer.h"
#include "Shader.h"
#include "Zero/Core/Core.h"
#include "Zero/Renderer/Framebuffer.h"
#include "Zero/Renderer/Viewer.hpp"
#include "Drawable2D.hpp"

#include "BatchManager.h"
#include "Shape.hpp"

namespace zr
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 ViewMatrix;
		Ref<Shader> TextureShader;
		Ref<Framebuffer> Framebuffer;
		Ref<Texture2D> WhiteTexture;
		Ref<BatchManager> BatchManager;
	};

	static Renderer2DStorage* sData;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void BeginScene(const Ref<Viewer>& viewer);
		static void BeginScene(const Ref<OrthographicCamera>& camera, const Ref<Framebuffer>& framebuffer);
		static void EndScene();

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor = glm::vec2(1.f, 1.f), const glm::vec4& tintingColor = glm::vec4(1.f, 1.f, 1.f, 1.f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor = glm::vec2(1.f, 1.f), const glm::vec4& tintingColor = glm::vec4(1.f, 1.f, 1.f, 1.f));

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor = glm::vec2(1.f, 1.f), const glm::vec4& tintingColor = glm::vec4(1.f, 1.f, 1.f, 1.f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& textureScalingFactor = glm::vec2(1.f, 1.f), const glm::vec4& tintingColor = glm::vec4(1.f, 1.f, 1.f, 1.f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const std::vector<glm::vec4>& colors, const glm::vec2& textureCoordinates, unsigned textureId);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::vector<glm::vec4>& colors, const glm::vec2& textureCoordinates, unsigned textureId);

		static void DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCoordinates, const std::vector<unsigned>& indices, unsigned textureId, const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec2& scalingFactor = glm::vec2(1.f));
		static void DrawQuad(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors, const std::vector<unsigned>& indices);

		static void DrawShape(const Ref<Shape> shape, const glm::vec2& position, float scale);
		static void DrawShape(const Ref<Shape> shape, const glm::vec3& position, float scale);

		static void DrawRotatedShape(const Ref<Shape>& shape, const glm::vec2& position, float scale, float angle);
		static void DrawRotatedShape(const Ref<Shape>& shape, const glm::vec3& position, float scale, float angle);

		// -------------- Draw classes ---------------
		//template <typename VertexType, typename IndexType>
		static void Draw(const Drawable2D& drawable);

	private:
		static void DrawShape(const Ref<Shape>& shape, const glm::mat4& transform);
	};

	/*template<typename VertexType, typename IndexType>
	inline void Renderer2D::Draw(const Drawable<VertexType, IndexType>& drawable)
	{
		const auto& objectVertices = drawable.getVertices();
		const auto& objectIndices = drawable.getIndices();
		const auto& colors = drawable.getColors();

		std::vector<BatchVertexTypes::ColoredVertex> innerVertices;
		for (unsigned i = 0; i < objectVertices.size(); ++i) {
			innerVertices.emplace_back(objectVertices[i], colors[i]);
		}

		sData->BatchManager->addVertices(innerVertices, objectIndices);
	}*/
}
