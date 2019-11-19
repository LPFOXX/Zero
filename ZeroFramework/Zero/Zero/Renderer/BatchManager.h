#pragma once

#include "Batch.h"

namespace zr
{
	class BatchManager
	{
	public:
		BatchManager() :
			mColoredVertexBatches(),
			mExtendedVertexBatches()
		{
			std::string batchVertexSrc = R"(
				#version 330 core
				layout(location = 0) in vec3 aPosition;
				layout(location = 1) in vec4 aColor;
				uniform mat4 uViewProjection;
				out vec4 vColor;
				void main()
				{
					vColor = aColor;
					gl_Position = uViewProjection * vec4(aPosition, 1.0);	
				}
			)";
			std::string batchFragmentSrc = R"(
				#version 330 core
				out vec4 color;
				in vec4 vColor;

				void main()
				{
					color = vColor;
				}
			)";
			std::string batchTextureVertexSrc = R"(
				#version 330 core
				layout(location = 0) in vec3 aPosition;
				layout(location = 1) in vec2 aTextureCoordinates;
				uniform mat4 uViewProjection;

				out vec2 vTextureCoordinates;

				void main()
				{
					vTextureCoordinates = aTextureCoordinates;
					gl_Position = uViewProjection * vec4(aPosition, 1.0);	
				}
			)";
			std::string batchTextureFragmentSrc = R"(
				#version 330 core
				out vec4 color;
				uniform vec4 uColor;

				in vec2 vTextureCoordinates;
				uniform sampler2D uTexture;

				void main()
				{
					color = uColor * texture(uTexture, vTextureCoordinates);
				}
			)";

			mBatchPositionShader = Shader::Create();
			mBatchPositionShader->loadFromStrings("BatchManagerPositionShader", batchVertexSrc, batchFragmentSrc);
			mBatchTextureShader = Shader::Create();
			mBatchTextureShader->loadFromStrings("BatchManagerTextureShader", batchTextureVertexSrc, batchTextureFragmentSrc);
		}

		virtual ~BatchManager()
		{

		}

		void addVertices(const std::vector<BatchVertexTypes::ColoredVertex>& vertices, const std::vector<unsigned> verticesIndices)
		{
			if (mColoredVertexBatches.empty()) {
				// No batch at all: create one
				ColoredVertexBatch cvb;
				cvb.addQuadVertices(vertices, verticesIndices);
				mColoredVertexBatches.push_back(cvb);
			}
			else {
				// search for a batch with enough storage room
				auto& it = std::find_if(mColoredVertexBatches.begin(), mColoredVertexBatches.end(), [&vertices](const ColoredVertexBatch& batch) {
					return batch.hasEnoughRoomFor(vertices);
				});

				if (it != mColoredVertexBatches.end()) {
					// There's a batch with enough room: add these vertices to it
					it->addQuadVertices(vertices, verticesIndices);
				}
				else {
					// There is no batch with enough room: create one batch and add these
					// vertices to it
					ColoredVertexBatch cvb;
					cvb.addQuadVertices(vertices, verticesIndices);
					mColoredVertexBatches.push_back(cvb);
				}
			}
		}

		void addVertices(const std::vector<BatchVertexTypes::ExtendedVertex>& vertices, const std::vector<unsigned> verticesIndices, unsigned textureId)
		{
			if (mExtendedVertexBatches.empty()) {
				// No batch at all: create one
				ExtendedVertexBatch evb(textureId);
				evb.addQuadVertices(vertices, verticesIndices);
				mExtendedVertexBatches.push_back(evb);
			}
			else {
				// search for a batch with enough storage room
				auto& it = std::find_if(mExtendedVertexBatches.begin(), mExtendedVertexBatches.end(), [&textureId, &vertices](const ExtendedVertexBatch& batch) {
					return batch.hasEnoughRoomFor(textureId, vertices);
				});

				if (it != mExtendedVertexBatches.end()) {
					// There's a batch with enough room: add these vertices to it
					it->addQuadVertices(vertices, verticesIndices);
				}
				else {
					// There is no batch with enough room: create one batch and add these
					// vertices to it
					ExtendedVertexBatch evb(textureId);
					evb.addQuadVertices(vertices, verticesIndices);
					mExtendedVertexBatches.push_back(evb);
				}
			}
		}

		void flush(const glm::mat4& viewProjectionMatrix)
		{
			if (!mColoredVertexBatches.empty()) {
				mBatchPositionShader->bind();
				mBatchPositionShader->setUniform("uViewProjection", viewProjectionMatrix);
				for (auto& batch : mColoredVertexBatches) {
					batch.flush();
				}
			}

			if (!mExtendedVertexBatches.empty()) {
				mBatchTextureShader->bind();
				mBatchTextureShader->setUniform("uTexture", 0);
				mBatchPositionShader->setUniform("uViewProjection", viewProjectionMatrix);
				mBatchPositionShader->setUniform("uColor", { 1.f, 1.f, 1.f, 1.f });
				for (auto& batch : mExtendedVertexBatches) {
					batch.flush(mBatchTextureShader);
				}
			}
		}

	private:
		std::vector<ColoredVertexBatch> mColoredVertexBatches;
		std::vector<ExtendedVertexBatch> mExtendedVertexBatches;

		Ref<Shader> mBatchPositionShader;
		Ref<Shader> mBatchTextureShader;
	};
}

#include "BatchManager.inl"
