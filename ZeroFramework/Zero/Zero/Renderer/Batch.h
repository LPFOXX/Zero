#pragma once

#include "../Core/Core.h"
#include "Buffer.h"
#include "Texture.h"

namespace zr
{
	namespace BatchVertexTypes
	{
		struct Vertex
		{
			Vertex(const glm::vec3& position) :
				Position(position)
			{

			}

			glm::vec3 Position;
		};

		struct ColoredVertex : Vertex
		{
			ColoredVertex(const glm::vec3& position, const glm::vec4& color) :
				Vertex(position),
				Color(color)
			{

			}

			glm::vec4 Color;
		};

		struct ExtendedVertex : Vertex
		{
			ExtendedVertex(const glm::vec3& position, const glm::vec2& textureCoordinates) :
				Vertex(position),
				TextureCoordinates(textureCoordinates)
			{

			}

			glm::vec2 TextureCoordinates;
		};
	}

	template <typename VertexType>
	class Batch
	{
	public:
		Batch();
		virtual ~Batch();

		virtual void addQuadVertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec4>& colors, const std::vector<unsigned>& vertexIndeces);
		virtual void addQuadVertices(const std::vector<VertexType>& vertices, const std::vector<unsigned>& verticesIndices);
		virtual void addVertex(const glm::vec3& position, const glm::vec2& textureCoordinates, const glm::vec4& color, unsigned vertexIndex);
		virtual bool hasEnoughRoomFor(const std::vector<VertexType>& vertices) const;
		virtual bool hasEnoughRoomFor(unsigned verticesCount) const;
		virtual void flush();

	protected:
		const unsigned kMaxNumVertices;

	protected:
		Ref<VertexArray> mVAO;

	private:
		std::vector<VertexType> mVertices;
		std::vector<unsigned> mIndices;
	};
	
	class ColoredVertexBatch : public Batch<BatchVertexTypes::ColoredVertex>
	{
	public:
		ColoredVertexBatch() :
			Batch()
		{
			mVAO = VertexArray::Create();
			Ref<VertexBuffer> batchVBO = VertexBuffer::Create(nullptr, kMaxNumVertices * sizeof(BatchVertexTypes::ColoredVertex), DrawMode::Dynamic);
			batchVBO->setLayout({
				{ ShaderDataType::Float3, "aPosition"},
				{ ShaderDataType::Float4, "aColor"} });

			mVAO->addVertexBuffer(batchVBO);

			Ref<IndexBuffer> batchEBO = IndexBuffer::Create(nullptr, kMaxNumVertices * 6U, DrawMode::Dynamic);
			mVAO->setIndexBuffer(batchEBO);
		}

		virtual ~ColoredVertexBatch()
		{

		}
	};

	class ExtendedVertexBatch : public Batch<BatchVertexTypes::ExtendedVertex>
	{
	public:
		ExtendedVertexBatch(unsigned textureId) :
			Batch(),
			mTextureId(textureId)
		{
			mVAO = VertexArray::Create();
			Ref<VertexBuffer> batchVBO = VertexBuffer::Create(nullptr, kMaxNumVertices * sizeof(BatchVertexTypes::ExtendedVertex), DrawMode::Dynamic);
			batchVBO->setLayout({
				{ ShaderDataType::Float3, "aPosition"},
				{ ShaderDataType::Float2, "aTextureCoordinates"}
				});

			mVAO->addVertexBuffer(batchVBO);

			Ref<IndexBuffer> batchEBO = IndexBuffer::Create(nullptr, kMaxNumVertices * 6U, DrawMode::Dynamic);
			mVAO->setIndexBuffer(batchEBO);
		}

		virtual ~ExtendedVertexBatch()
		{

		}

		virtual bool hasEnoughRoomFor(unsigned textureId, const std::vector<BatchVertexTypes::ExtendedVertex>& vertices) const
		{
			if (mTextureId != textureId)
				return false;

			return Batch::hasEnoughRoomFor(vertices);
		}

		virtual bool hasEnoughRoomFor(unsigned textureId, unsigned verticesCount) const
		{
			if (textureId != mTextureId) {
				return false;
			}

			return Batch::hasEnoughRoomFor(verticesCount);
		}

		virtual void flush(const Ref<Shader>& shader)
		{
			Texture2D::ActivateTextureSlot(0, mTextureId);
			Batch::flush();
		}

	private:
		unsigned mTextureId;
	};
}

#include "Batch.inl"
