#pragma once

#include "../Core/Core.h"
#include "../Core/Profiller.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shape.hpp"
#include "RenderCommand.h"
#include "RendererAPI.h"

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
		std::vector<unsigned int> mIndices;
	};

	class ColoredVertexBatch : public Batch<BatchVertexTypes::ColoredVertex>
	{
	public:
		ColoredVertexBatch() :
			Batch()
		{
			ZR_PROFILER_FUNCTION();
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
			ZR_PROFILER_FUNCTION();
		}
	};

	class ExtendedVertexBatch : public Batch<BatchVertexTypes::ExtendedVertex>
	{
	public:
		ExtendedVertexBatch(unsigned textureId, const glm::vec4& color, const glm::vec2& scalingFactor = glm::vec2(1.f)) :
			Batch(),
			mTextureId(textureId),
			mColor(color),
			mScalingFactor(scalingFactor)
		{
			ZR_PROFILER_FUNCTION();
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
			ZR_PROFILER_FUNCTION();
		}

		virtual bool hasEnoughRoomFor(unsigned textureId, const std::vector<BatchVertexTypes::ExtendedVertex>& vertices, const glm::vec4& color) const
		{
			if (mTextureId != textureId || mColor != color)
				return false;

			return Batch::hasEnoughRoomFor(vertices);
		}

		virtual bool hasEnoughRoomFor(unsigned textureId, unsigned verticesCount, const glm::vec4& color) const
		{
			if (textureId != mTextureId || mColor != color) {
				return false;
			}

			return Batch::hasEnoughRoomFor(verticesCount);
		}

		virtual void flush(const Ref<Shader>& shader)
		{
			Texture2D::ActivateTextureSlot(0, mTextureId);
			shader->setUniform("uColor", mColor);
			shader->setUniform("uTextureScalingFactor", mScalingFactor);
			Batch::flush();
		}

	private:
		unsigned mTextureId;
		glm::vec4 mColor;
		glm::vec2 mScalingFactor;
	};

	class ShapeVertexBatch
	{
	public:
		struct ShapeVertexBounds
		{
			ShapeVertexBounds(unsigned offset, unsigned count) :
				Offset(offset),
				Count(count)
			{

			}

			unsigned Offset;
			unsigned Count;
		};

	public:
		ShapeVertexBatch(RendererAPI::DrawPrimitive primitiveType) :
			kMaxNumVertices(1048576U / sizeof(BatchVertexTypes::ColoredVertex)), // Enough storage for 1MB worth of data
			mPrimitiveType(primitiveType)
		{
			ZR_PROFILER_FUNCTION();
			mVAO = VertexArray::Create();
			Ref<VertexBuffer> batchVBO = VertexBuffer::Create(nullptr, kMaxNumVertices * sizeof(BatchVertexTypes::ColoredVertex), DrawMode::Dynamic);
			batchVBO->setLayout({
				{ ShaderDataType::Float3, "aPosition"},
				{ ShaderDataType::Float4, "aColor"} });

			mVAO->addVertexBuffer(batchVBO);
		}

		virtual ~ShapeVertexBatch()
		{
		}

		bool hasEnoughRoom(const std::vector<BatchVertexTypes::ColoredVertex>& vertices, RendererAPI::DrawPrimitive primitiveType) const
		{
			return (mPrimitiveType == primitiveType) && (mVertices.size() + vertices.size() <= kMaxNumVertices);
		}

		bool hasEnoughRoom(unsigned verticesCount, RendererAPI::DrawPrimitive primitiveType) const
		{
			return (mPrimitiveType == primitiveType) && (mVertices.size() + verticesCount <= kMaxNumVertices);
		}

		void addVertices(const std::vector<BatchVertexTypes::ColoredVertex>& vertices)
		{
			mVertexBounds.emplace_back(mVertices.size(), vertices.size());
			mVertices.insert(mVertices.end(), vertices.begin(), vertices.end());
		}

		void flush()
		{
			mVAO->bind();
			const Ref<VertexBuffer>& vb = mVAO->getVertexBuffers()[0]; // Using a single VBO for the vertices
			vb->setData(&mVertices[0].Position.x, mVertices.size() * sizeof(BatchVertexTypes::ColoredVertex));

			// Draw ranged
			//RenderCommand::DrawIndexed(mVAO);
			for (auto& vertexBounds : mVertexBounds) {
				RenderCommand::DrawArrays(mPrimitiveType, vertexBounds.Offset, vertexBounds.Count);
			}

			mVertexBounds.clear();
			mVertices.clear();
		}

	private:
		const unsigned kMaxNumVertices;
		RendererAPI::DrawPrimitive mPrimitiveType;
		Ref<VertexArray> mVAO;
		std::vector<BatchVertexTypes::ColoredVertex> mVertices;
		std::vector<ShapeVertexBounds> mVertexBounds;
	};
}

#include "Batch.inl"
