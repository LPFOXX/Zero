#pragma once

#include "Batch.h"
#include "RendererAPI.h"

namespace zr
{
	class BatchManager
	{
	public:
		BatchManager() :
			mColoredVertexBatches(),
			mExtendedVertexBatches(),
			mShapesVertexBatch()
		{
			ZR_PROFILER_FUNCTION();
			mBatchPositionShader = Shader::Create();
			mBatchPositionShader->loadFromFile("resources/shaders/Batch.glsl");
			mBatchTextureShader = Shader::Create();
			mBatchTextureShader->loadFromFile("resources/shaders/BatchTexture.glsl");
			mBatchTextureShader->bind();
			mBatchTextureShader->setUniform("uTexture", 0);
		}

		virtual ~BatchManager()
		{
			ZR_PROFILER_FUNCTION();
		}

		void addVertices(const std::vector<BatchVertexTypes::ColoredVertex>& vertices, const std::vector<unsigned>& verticesIndices)
		{
			// search for a batch with enough storage room
			auto it = std::find_if(mColoredVertexBatches.begin(), mColoredVertexBatches.end(), [&vertices](const ColoredVertexBatch& batch) {
				return batch.hasEnoughRoomFor(vertices);
			});

			if (it != mColoredVertexBatches.end()) {
				// There's a batch with enough room: add these vertices to it
				it->addVertices(vertices, verticesIndices);
			}
			else {
				// There is no batch with enough room: create one batch and add these
				// vertices to it
				ColoredVertexBatch cvb;
				cvb.addVertices(vertices, verticesIndices);
				mColoredVertexBatches.push_back(cvb);
			}
		}

		void addVertices(const std::vector<BatchVertexTypes::ColoredVertex>& vertices, const std::vector<unsigned>& verticesIndices, RendererAPI::DrawPrimitive primitiveType)
		{
			auto it = std::find_if(mShapesVertexBatch.begin(), mShapesVertexBatch.end(), [&vertices, &primitiveType](const ShapeVertexBatch& batch) {
				return batch.hasEnoughRoom(vertices, primitiveType);
			});

			if (it != mShapesVertexBatch.end()) {
				// There's a batch with enough room: add these vertices to it
				//it->addVertices(vertices);
				it->addVertices(vertices, verticesIndices);
			}
			else {
				// There is no batch with enough room: create one batch and add these
				// vertices to it
				ShapeVertexBatch svb(primitiveType);
				svb.addVertices(vertices, verticesIndices);
				mShapesVertexBatch.emplace_back(svb);
			}
		}

		void addVertices(const std::vector<BatchVertexTypes::ExtendedVertex>& vertices, const std::vector<unsigned>& verticesIndices, unsigned textureId, const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec2& scalingFactor = glm::vec2(1.f))
		{
			if (mExtendedVertexBatches.empty()) {
				// No batch at all: create one
				ExtendedVertexBatch evb(textureId, color, scalingFactor);
				evb.addVertices(vertices, verticesIndices);
				mExtendedVertexBatches.push_back(evb);
			}
			else {
				// search for a batch with enough storage room
				auto it = std::find_if(mExtendedVertexBatches.begin(), mExtendedVertexBatches.end(), [&textureId, &vertices, &color](const ExtendedVertexBatch& batch) {
					return batch.hasEnoughRoomFor(textureId, vertices, color);
				});

				if (it != mExtendedVertexBatches.end()) {
					// There's a batch with enough room: add these vertices to it
					it->addVertices(vertices, verticesIndices);
				}
				else {
					// There is no batch with enough room: create one batch and add these
					// vertices to it
					ExtendedVertexBatch evb(textureId, color, scalingFactor);
					evb.addVertices(vertices, verticesIndices);
					mExtendedVertexBatches.push_back(evb);
				}
			}
		}

		void flush(const glm::mat4& viewProjectionMatrix)
		{
			ZR_PROFILER_FUNCTION();
			if (!mColoredVertexBatches.empty()) {
				mBatchPositionShader->bind();
				mBatchPositionShader->setUniform("uViewProjection", viewProjectionMatrix);
				for (auto& batch : mColoredVertexBatches) {
					batch.flush();
				}
			}

			// shapes
			if (!mShapesVertexBatch.empty()) {
				mBatchPositionShader->bind();
				mBatchPositionShader->setUniform("uViewProjection", viewProjectionMatrix);
				for (auto& batch : mShapesVertexBatch) {
					batch.flush();
				}
			}

			// textures
			if (!mExtendedVertexBatches.empty()) {
				mBatchTextureShader->bind();
				mBatchTextureShader->setUniform("uViewProjection", viewProjectionMatrix);
				for (auto& batch : mExtendedVertexBatches) {
					batch.flush(mBatchTextureShader);
				}
			}
		}

	private:
		std::vector<ColoredVertexBatch> mColoredVertexBatches;
		std::vector<ExtendedVertexBatch> mExtendedVertexBatches;
		std::vector<ShapeVertexBatch> mShapesVertexBatch;

		Ref<Shader> mBatchPositionShader;
		Ref<Shader> mBatchTextureShader;
	};
}

#include "BatchManager.inl"
