#pragma once

#include "Transformable.hpp"
#include "Drawable2D.hpp"

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class Quad : public Drawable2D, public Transformable2D
	{
	public:
		Quad() :
			Drawable2D(),
			Transformable2D()
		{
			mVertices.resize(4);
			mVertices[0] = { -.5f, -.5f, 0.f };	// Bottom Left
			mVertices[1] = { +.5f, -.5f, 0.f };	// Bottom Right
			mVertices[2] = { +.5f, +.5f, 0.f };	// Top Right
			mVertices[3] = { -.5f, +.5f, 0.f };	// Top Left

			mIndices.resize(6);
			mIndices = { 0, 1, 2, 2, 3, 0 };
		}

		virtual ~Quad() = default;

		/**
		* \brief Sets color for all four vertices of the quad.
		* - The opacity is the fourth channel of the color.
		*/
		void setColor(const glm::vec4& verticesColor)
		{
			if (mColors.size() != 4) mColors.resize(4);

			mColors[0] = verticesColor;
			mColors[1] = verticesColor;
			mColors[2] = verticesColor;
			mColors[3] = verticesColor;
		}

		/**
		* \brief Sets top and bottom color for the vertices of the quad.
		* - The opacity is the fourth channel of the color.
		*/
		void setColor(const glm::vec4& topColor, const glm::vec4& bottomColor)
		{
			if (mColors.size() != 4) mColors.resize(4);

			mColors[0] = bottomColor;
			mColors[1] = bottomColor;

			mColors[2] = topColor;
			mColors[3] = topColor;
		}

		/**
		* \brief Sets the color of each indevidual vertex of the quad.
		* - The order goes: top left, top right, bottom right and bottom left.
		* - The opacity is the fourth channel of the color.
		*/
		void setColor(const glm::vec4& topLeft, const glm::vec4& topRight, const glm::vec4& bottomRight, const glm::vec4& bottomLeft)
		{
			if (mColors.size() != 4) mColors.resize(4);

			mColors[0] = bottomLeft;
			mColors[1] = bottomRight;
			mColors[2] = topRight;
			mColors[3] = topLeft;
		}

		virtual void transformVertices() const override
		{
			const auto& transform = getTransformMatrix();
			mVertices[0] = transform * glm::vec4(-.5f, -.5f, .0f, 1.f);
			mVertices[1] = transform * glm::vec4(+.5f, -.5f, .0f, 1.f);
			mVertices[2] = transform * glm::vec4(+.5f, +.5f, .0f, 1.f);
			mVertices[3] = transform * glm::vec4(-.5f, +.5f, .0f, 1.f);
		}
	};
}
