#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include "../Observer.h"

namespace zr
{
	class ViewportResizeListener : public Observer<glm::vec2>
	{
	public:
		ViewportResizeListener() :
			Observer()
		{

		}

		virtual ~ViewportResizeListener()
		{

		}

		virtual void update(const glm::vec2& viewportSize)
		{
			onViewportUpdate(viewportSize);
		}

	protected:
		virtual void onViewportUpdate(const glm::vec2& viewportSize) = 0;
	};

	class ViewportDependable : public Observable<ViewportDependable, glm::vec2>
	{
	public:
		ViewportDependable();
		virtual ~ViewportDependable();

		/**
		* @brief Updates the viewport size value stored by the object.
		*
		* @param viewportSize The size of the viewport.
		*
		*/
		virtual void viewportUpdate(const glm::uvec2& viewportSize);

		/**
		* @brief Updates the viewport size value stored by the object.
		*
		* @param width The new width of the viewport.
		* @param height The new height of the viewport.
		*
		*/
		virtual void viewportUpdate(unsigned width, unsigned height);
	
	public:
		static inline const glm::vec2& GetViewportSize()
		{
			return ViewportDependable::sViewportSize;
		}

	protected:
		static glm::vec2 sViewportSize;
	};
}
