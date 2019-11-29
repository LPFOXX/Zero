#include "../../zr_pch.h"

#include "../Core/Log.h"
#include "../Core/Profiller.h"

namespace zr
{
	template <typename VertexType>
	inline Batch<VertexType>::Batch() :
		kMaxNumVertices(1048576U / sizeof(VertexType)), // Enough storage for 1MB worth of data
		mVAO(nullptr),
		mVertices(),
		mIndices()
	{
		ZR_PROFILER_FUNCTION();
	}

	template <typename VertexType>
	inline Batch<VertexType>::~Batch()
	{
		ZR_PROFILER_FUNCTION();
	}

	template <typename VertexType>
	inline void Batch<VertexType>::addQuadVertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCoordinates, const std::vector<glm::vec4>& colors, const std::vector<unsigned>& vertexIndeces)
	{
		ZR_PROFILER_FUNCTION();
		if (positions.size() != 4 || textureCoordinates.size() != 4 || vertexIndeces.size() != 6) {
			return;
		}

		for (unsigned i = 0; i < 4; ++i) {
			/*mPositions.push_back(positions[i]);
			mTextureCoordinates.push_back(textureCoordinates[i]);

			if (colors.size() == 4) {
				mColors.push_back(colors[i]);
			}
			else if (colors.size() == 0) {
				mColors.push_back({ 1.f, 1.f, 1.f, 1.f });
			}
			else if (colors.size == 1) {
				mColors.push_back(colors[0]);
			}*/
		}

		if (colors.size() == 2) {
			/*mColors.push_back(colors[0]);
			mColors.push_back({1.f, 1.f, 1.f, 1.f});
			mColors.push_back(colors[1]);
			mColors.push_back({1.f, 1.f, 1.f, 1.f});*/
		}
		else if (colors.size() == 3) {
			/*mColors.push_back(colors[0]);
			mColors.push_back(colors[1]);
			mColors.push_back(colors[2]);
			mColors.push_back({ 1.f, 1.f, 1.f, 1.f });*/
		}

		unsigned indexCount = mVertices.size();
		for (unsigned i = 0; i < 6; ++i) {
			mIndices.push_back(vertexIndeces[i] + indexCount);
		}
	}

	template<typename VertexType>
	inline void Batch<VertexType>::addQuadVertices(const std::vector<VertexType>& vertices, const std::vector<unsigned>& verticesIndices)
	{
		ZR_PROFILER_FUNCTION();
		/*if (vertices.size() != 4 || verticesIndices.size() != 6) {
			ZR_CORE_ERROR("Quad has 4 vertices and 6 indices. Data received has {0} vertices and {1} indices.", vertices.size(), verticesIndices.size());
			return;
		}

		if (mVertices.size() + vertices.size() > kMaxNumVertices) {
			ZR_CORE_ERROR("Not enough storage to add {0} vertices", vertices.size());
			return;
		}*/

		std::size_t indexCount = mVertices.size();
		for (unsigned index : verticesIndices) {
			mIndices.push_back((unsigned)indexCount + index);
		}

		for (auto& vertex : vertices) {
			mVertices.push_back(vertex);
		}
	}

	template <typename VertexType>
	inline void Batch<VertexType>::addVertex(const glm::vec3& position, const glm::vec2& textureCoordinates, const glm::vec4& color, unsigned vertexIndex)
	{
		ZR_PROFILER_FUNCTION();
		/*	mPositions.push_back(position);
			mTextureCoordinates.push_back(textureCoordinates);
			mColors.push_back(color);*/
		mIndices.push_back(vertexIndex + mVertices.size());
	}

	template <typename VertexType>
	inline bool Batch<VertexType>::hasEnoughRoomFor(const std::vector<VertexType>& vertices) const
	{
		return (mVertices.size() + vertices.size() <= kMaxNumVertices);
	}

	template <typename VertexType>
	inline bool Batch<VertexType>::hasEnoughRoomFor(unsigned verticesCount) const
	{
		return (mVertices.size() + verticesCount <= kMaxNumVertices);
	}

	template <typename VertexType>
	inline void Batch<VertexType>::flush()
	{
		ZR_PROFILER_FUNCTION();
		mVAO->bind();
		const Ref<VertexBuffer>& vb = mVAO->getVertexBuffers()[0]; // Using a single VBO for the vertices
		vb->setData(&mVertices[0].Position.x, mVertices.size() * sizeof(VertexType));
		mVAO->getIndexBuffer()->setData(&mIndices[0], mIndices.size());

		RenderCommand::DrawIndexed(mVAO);

		mVertices.clear();
		mIndices.clear();
	}
}
