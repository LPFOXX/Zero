#include <zr_pch.h>

#include "KeyEvent.h"

namespace zr
{
	KeyEvent::KeyEvent(int keyCode) :
		Event(),
		mKeyCode(keyCode)
	{
		
	}
	
	KeyEvent::~KeyEvent()
	{
		
	}
	
	unsigned KeyEvent::getFamilyFlags() const
	{
		return EventFamily::InputEvent | EventFamily::KeyboadEvent;
	}
}
