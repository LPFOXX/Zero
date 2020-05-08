#pragma once

#include <vector>

namespace zr
{
	template <typename VertexType, typename IndexType, typename ColorType>
	class Drawable
	{
	public:
		Drawable() :
			mVertices(),
			mIndices(),
			mColors()
		{

		}

		const std::vector<VertexType>& getVertices() const
		{
			transformVertices();

			return mVertices;
		}

		const std::vector<IndexType>& getIndices() const
		{
			return mIndices;
		}

		const std::vector<ColorType>& getColors() const
		{
			return mColors;
		}

		virtual void draw() const = 0;

	protected:
		virtual void transformVertices() const = 0;

	protected:
		mutable std::vector<VertexType> mVertices;
		std::vector<IndexType> mIndices;
		std::vector<ColorType> mColors;
	};
}
