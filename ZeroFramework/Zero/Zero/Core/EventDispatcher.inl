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

	template<typename T, typename F>
	inline bool EventDispatcher::dispatch(const F& func)
	{
		if (mEvent.getType() == T::GetStaticType()) {
			mEvent.setHandled(func(static_cast<T&>(mEvent)));
			return true;
		}
		return false;
	}
}
