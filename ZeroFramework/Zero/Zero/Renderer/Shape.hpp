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

		inline RendererAPI::DrawPrimitive getPrimitiveType()
		{
			return mPrimitiveType;
		}

	protected:
		std::vector<glm::vec3> mVertices;
		RendererAPI::DrawPrimitive mPrimitiveType;
	};

	class RegularShape : public Shape
	{
	public:
		RegularShape(unsigned vertexCount) :
			Shape(RendererAPI::DrawPrimitive::TriangleFan)
		{
			if (vertexCount >= 3) {
				mVertices.reserve(1 + vertexCount); // one for the central vertex

				float sideAngle = 360.f / (float)vertexCount;

				// central vertex
				mVertices.emplace_back(0.f, 0.f, 0.f);

				for (unsigned i = 0U; i < vertexCount; ++i) {
					mVertices.emplace_back(std::cos(glm::radians(90.f + sideAngle * i)) * .5f, std::sin(glm::radians(90.f + sideAngle * i)) * .5f, 0.f);
				}

				mVertices.emplace_back(0.f, .5f, 0.f);
			}
		}
	};
}
