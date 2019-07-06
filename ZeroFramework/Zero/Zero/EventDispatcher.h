#pragma once

#include "Events.h"

namespace zr
{
	template <typename T>
	using EventFunction = std::function<bool(T&)>;
	
	class EventDispatcher
	{
	public:
		inline EventDispatcher(Event& event);
		inline virtual ~EventDispatcher();

		template <typename T>
		inline bool dispatch(EventFunction<T> func);

	private:
		Event& mEvent;
	};
}

#include "EventDispatcher.inl"
