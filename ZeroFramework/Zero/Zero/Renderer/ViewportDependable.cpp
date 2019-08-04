#include <zr_pch.h>

#include "ViewportDependable.h"

namespace zr
{
	glm::vec2 ViewportDependable::sViewportSize = glm::vec2(0.f, 0.f);

	ViewportDependable::ViewportDependable()
	{
	}

	ViewportDependable::~ViewportDependable()
	{
	}

	void ViewportDependable::viewportUpdate(const glm::uvec2& viewportSize)
	{
		viewportUpdate(viewportSize.x, viewportSize.y);
	}

	void ViewportDependable::viewportUpdate(unsigned width, unsigned height)
	{
		if (ViewportDependable::sViewportSize.x != width || ViewportDependable::sViewportSize.y != height) {
			ViewportDependable::sViewportSize.x = width;
			ViewportDependable::sViewportSize.y = height;

			notify(ViewportDependable::sViewportSize);
		}
	}
};