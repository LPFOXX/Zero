#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include "RendererAPI.h"

namespace zr
{
	class Shape
	{
	public:
		Shape(RendererAPI::DrawPrimitive primitiveType) :
			mVertices(),
			mPrimitiveType(primitiveType)
		{

		}

		virtual ~Shape() = default;

		inline const std::vector<glm::vec3>& getVertices()
		{
			return mVertices;
		}

		inline const std::vector<unsigned>& getIndices()
		{
			return mIndices;
		}

		inline RendererAPI::DrawPrimitive getPrimitiveType()
		{
			return mPrimitiveType;
		}

	protected:
		std::vector<glm::vec3> mVertices;
		std::vector<unsigned> mIndices;
		RendererAPI::DrawPrimitive mPrimitiveType;
	};

	class RegularShape : public Shape
	{
	public:
		RegularShape(unsigned vertexCount) :
			Shape(RendererAPI::DrawPrimitive::TriangleFan)
		{
			if (vertexCount >= 3) {
				mVertices.reserve(1 + vertexCount);		// one for the central vertex
				mIndices.reserve(1 + 1 + vertexCount);	// one for the central vertex + one for the first vertex

				float sideAngle = 360.f / (float)vertexCount;

				// central vertex
				mVertices.emplace_back(0.f, 0.f, 0.f);
				mIndices.emplace_back(0);

				float initialAngleOffset = 90.f;
				if (vertexCount % 2 == 0) {
					initialAngleOffset = .5f * sideAngle;
				}

				for (unsigned i = 0U; i < vertexCount; ++i) {
					mVertices.emplace_back(std::cos(glm::radians(initialAngleOffset + sideAngle * i)) * .5f, std::sin(glm::radians(initialAngleOffset + sideAngle * i)) * .5f, 0.f);
					mIndices.emplace_back(i + 1);
				}

				mIndices.emplace_back(1); // close the shape, since it's drawn via triangle fans
			}
		}
	};
}
