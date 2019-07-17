#include <zr_pch.h>

#include "Timer.h"

namespace zr
{
	Timer::Timer() :
		mTime(Time::Zero()),
		mRestartPoint(std::chrono::steady_clock::now())
	{

	}

	Timer::~Timer()
	{

	}

	const Time& Timer::getTime() const
	{
		mTime = Time(std::chrono::steady_clock::now() - mRestartPoint);
		return mTime;
	}

	Time Timer::restart()
	{
		Time aux = mTime;
		mTime = Time(std::chrono::steady_clock::now() - mRestartPoint);
		mRestartPoint = std::chrono::steady_clock::now();
		return aux;
	}
}
