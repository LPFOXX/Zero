#include <zr_pch.h>

#include "Clock.h"

namespace zr
{
	Time::Time() :
		mTimeInMicroseconds(0LL)
	{

	}
	
	Time::~Time()
	{

	}

	Clock::Clock() :
		mLastRestartPoint(Time::Now())
	{

	}

	Clock::~Clock()
	{
	}
}
