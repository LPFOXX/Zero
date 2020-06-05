#pragma once

#include "Events.h"

namespace zr
{
	class EventDispatcher
	{
	public:
		inline EventDispatcher(Event& event) :
			mEvent(event)
		{
		}

		inline virtual ~EventDispatcher() = default;

		template <typename T, typename F>
		inline bool dispatch(const F& func)
		{
			if (mEvent.getType() == T::GetStaticType()) {
				mEvent.setHandled(func(static_cast<T&>(mEvent)));
				return true;
			}
			return false;
		}

	private:
		Event& mEvent;
	};
}
