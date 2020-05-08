#include <zr_pch.h>

#include "Drawable2D.hpp"
#include "Renderer2D.h"

namespace zr
{
	void Drawable2D::draw() const
	{
		Renderer2D::Draw(*this);
	}
}
