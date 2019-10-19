#include <zr_pch.h>

namespace zr
{
	inline EventDispatcher::EventDispatcher(Event& event) :
		mEvent(event)
	{

	}

	inline EventDispatcher::~EventDispatcher()
	{

	}

	template<typename T>
	inline bool EventDispatcher::dispatch(EventFunction<T> func)
	{
		if (mEvent.getType() == T::GetStaticType()) {
			mEvent.setHandled(func(*(T*)&mEvent));
			return true;
		}
		return false;
	}
}
