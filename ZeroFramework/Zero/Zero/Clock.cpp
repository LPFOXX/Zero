#include <zr_pch.h>

#include "Clock.h"

namespace zr
{
	Time::Time() :
		mTime(Time::seconds::zero())
	{

	}

	Time::Time(float timeInSeconds) :
		mTime(timeInSeconds)
	{

	}

	Time::Time(const Time::seconds& time) :
		mTime(time)
	{
	}
	
	Time::~Time()
	{

	}

	Time Time::Zero()
	{
		return Time(Time::seconds::zero());
	}

	Time Time::Now()
	{
		Time::seconds();
		return Time(0);
	}

	Clock::Clock() :
		mElapsedTime(Time::seconds::zero())
	{

	}

	Clock::Clock(const Time& time) :
		mElapsedTime(time)
	{
	}

	Clock::Clock(const Time::seconds& seconds) :
		mElapsedTime(seconds)
	{

	}

	Clock::Clock(float seconds) :
		mElapsedTime(seconds)
	{
	}

	Clock::~Clock()
	{
	}
}
