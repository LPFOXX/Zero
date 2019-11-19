#pragma once

#include "Events.h"

namespace zr
{
	class EventDispatcher
	{
	public:
		inline EventDispatcher(Event& event);
		inline virtual ~EventDispatcher();

		template <typename T, typename F>
		inline bool dispatch(const F& func);

	private:
		Event& mEvent;
	};
}

#include "EventDispatcher.inl"
