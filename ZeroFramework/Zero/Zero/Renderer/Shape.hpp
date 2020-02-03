#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include "RendererAPI.h"

namespace zr
{
	class Shape
	{
	public:
		class Outline
		{
		public:
			Outline(RendererAPI::DrawPrimitive drawPrimitive) :
				mDrawPrimitive(drawPrimitive),
				mThickness(0.f),
				mColor(0.f, 0.f, 0.f, 1.f),
				mVertices(),
				mIndices()
			{

			}

			virtual ~Outline() = default;

			void setThickness(float thickness)
			{
				if (thickness >= 0.f)
					mThickness = thickness;
			}

			float getThickness() const
			{
				return mThickness;
			}

			void setColor(const glm::vec4& color)
			{
				mColor = color;
			}

			const glm::vec4& getColor() const
			{
				return mColor;
			}

			void setData(const std::vector<glm::vec3>& vertices, const std::vector<unsigned>& indices)
			{
				mVertices = vertices;
				mIndices = indices;
			}

			void clear()
			{
				mVertices.clear();
				mIndices.clear();
			}

			const std::vector<glm::vec3>& getVertices() const
			{
				return mVertices;
			}

			const std::vector<unsigned>& getIndices() const
			{
				return mIndices;
			}

			RendererAPI::DrawPrimitive getPrimitiveType() const
			{
				return mDrawPrimitive;
			}

		private:
			RendererAPI::DrawPrimitive mDrawPrimitive;
			float mThickness;
			glm::vec4 mColor;
			std::vector<glm::vec3> mVertices;
			std::vector<unsigned> mIndices;
		};

	public:
		Shape(RendererAPI::DrawPrimitive primitiveType) :
			mVertices(),
			mPrimitiveType(primitiveType),
			mColor(1.f, 1.f, 1.f, 1.f),	// Default color is opaque white,
			mOutline(RendererAPI::DrawPrimitive::TriangleStrip),
			mVertexCount(0U)
		{
		}

		virtual ~Shape() = default;

		inline const std::vector<glm::vec3>& getVertices() const
		{
			return mVertices;
		}

		inline const std::vector<unsigned>& getIndices() const
		{
			return mIndices;
		}

		inline RendererAPI::DrawPrimitive getPrimitiveType() const
		{
			return mPrimitiveType;
		}

		inline const glm::vec4& getFillColor() const
		{
			return mColor;
		}

		void setFillColor(const glm::vec4& color)
		{
			mColor = color;
		}

		void setOutlineThickness(float thickness)
		{
			mOutline.setThickness(thickness);
			if (thickness > 0.f) {
				generateOutlineVertices();
			}
		}

		float getOutlineThickness() const
		{
			return mOutline.getThickness();
		}

		void setOutlineColor(const glm::vec4& color)
		{
			mOutline.setColor(color);
		}

		const glm::vec4& getOutlineColor() const
		{
			return mOutline.getColor();
		}

		void setOutlineParam(float thickness, const glm::vec4& color)
		{
			mOutline.setThickness(thickness);
			mOutline.setColor(color);
			generateOutlineVertices();
		}

		const Shape::Outline& getOutline() const
		{
			return mOutline;
		}

	public:
		static float ComputeTriangleHeight(float internalAngle, const glm::vec2& vertex0, const glm::vec2& vertex1)
		{
			float b = glm::length(glm::abs(vertex0 - vertex1));
			return b * std::sin(glm::radians(internalAngle));
		}

	protected:
		virtual void generateOutlineIndices(std::vector<unsigned>& outlineIndices, unsigned shapeVertexCount) = 0;
		virtual void computeVertexData() = 0;
		virtual void generateOutlineVertices() = 0;

	protected:
		std::vector<glm::vec3> mVertices;
		std::vector<unsigned> mIndices;
		RendererAPI::DrawPrimitive mPrimitiveType;
		glm::vec4 mColor;
		Shape::Outline mOutline;
		unsigned mVertexCount;
	};

	class RegularShape : public Shape
	{
	public:
		RegularShape(unsigned vertexCount) :
			Shape(RendererAPI::DrawPrimitive::TriangleFan)
		{
			if (vertexCount >= 3) {
				mVertexCount = vertexCount;
				mVertices.reserve(1 + vertexCount);		// one for the central vertex
				mIndices.reserve(1 + 1 + vertexCount);	// one for the central vertex + one for the first vertex
				generateInnerVertices();
			}
		}

		void setRadius(float radius)
		{

		}

	private:
		virtual void generateOutlineIndices(std::vector<unsigned>& outlineIndices, unsigned shapeVertexCount) override
		{
			unsigned first = shapeVertexCount;
			unsigned second = 0;
			bool insertOutterVertexFirst = true;

			for (unsigned i = 0; i < shapeVertexCount * 2; ++i) {
				outlineIndices.emplace_back(first);
				outlineIndices.emplace_back(second);
				unsigned next = ((first + 1) % (shapeVertexCount)) + (insertOutterVertexFirst ? shapeVertexCount : 0);
				outlineIndices.emplace_back(next);

				first = second;
				second = next;
				insertOutterVertexFirst = !insertOutterVertexFirst;
			}
		}

		virtual void computeVertexData() override
		{
			if (mVertexCount >= 3) {
				generateInnerVertices();
				generateOutlineVertices();
			}
		}

		virtual void generateOutlineVertices() override
		{
			mOutline.clear();
			float sideAngle = 360.f / (float)mVertexCount;
			float initialAngleOffset = 90.f;
			if (mVertexCount % 2 == 0) {
				initialAngleOffset = -.5f * sideAngle;
			}

			std::vector<glm::vec3> outlineVertices;
			outlineVertices.reserve(mVertexCount * 2);

			// inserts inner vertex into the outline shape 
			outlineVertices.insert(outlineVertices.end(), ++mVertices.begin(), mVertices.end());
			// computes outline outter vertex position
			for (unsigned i = 0U; i < mVertexCount; ++i) {
				outlineVertices.emplace_back(std::cos(glm::radians(initialAngleOffset + sideAngle * i)) * 1.1f, std::sin(glm::radians(initialAngleOffset + sideAngle * i)) * 1.1f, 0.f);
			}

			std::vector<unsigned> outlineIndices;
			outlineIndices.reserve(mVertexCount * 2 * 3);
			generateOutlineIndices(outlineIndices, mVertexCount);
			mOutline.setData(outlineVertices, outlineIndices);
		}

	private:
		void generateInnerVertices()
		{
			mVertices.clear();
			mIndices.clear();
			float sideAngle = 360.f / (float)mVertexCount;
			float initialAngleOffset = 90.f;
			if (mVertexCount % 2 == 0) {
				initialAngleOffset = -.5f * sideAngle;
			}

			// central vertex
			mVertices.emplace_back(0.f, 0.f, 0.f);
			mIndices.emplace_back(0);

			for (unsigned i = 0U; i < mVertexCount; ++i) {
				mVertices.emplace_back(std::cos(glm::radians(initialAngleOffset + sideAngle * i)), std::sin(glm::radians(initialAngleOffset + sideAngle * i)), 0.f);
				mIndices.emplace_back(i + 1);
			}

			// finish the shape, since it's drawn with triangle fans
			mIndices.emplace_back(1);
		}
	};
}
