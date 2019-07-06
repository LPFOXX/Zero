#include <zr_pch.h>

#include "Event.h"

namespace zr
{
	Event::Event() :
		mIsHandled(false)
	{
		
	}
	
	Event::~Event()
	{
		
	}
	
	std::string Event::toString() const
	{
		return getName();
	}

	void Event::setHandled(bool handled)
	{
		mIsHandled = handled;
	}
	
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}
