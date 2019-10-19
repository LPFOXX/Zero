#include <zr_pch.h>

#include "Timer.h"

namespace zr
{
	Timer::Timer() :
		mRestartTimePoint(Time::Now())
	{
	}

	Timer::~Timer()
	{
		auto elapsedTime = Time::Now() - mRestartTimePoint;
		std::cout << elapsedTime.asMicroseconds() << "us (" << elapsedTime.asMilliseconds() << "us\n";
	}

	Time Timer::restart()
	{
		Time aux = Time::Now() - mRestartTimePoint;
		mRestartTimePoint = Time::Now();
		return aux;
	}
}
