#pragma once

#include "Drawable.hpp"

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class Drawable2D : public Drawable<glm::vec3, uint32_t, glm::vec4>
	{
	public:
		Drawable2D() :
			Drawable()
		{

		}

		virtual ~Drawable2D() = default;

		virtual void draw() const override;	
	};
}
