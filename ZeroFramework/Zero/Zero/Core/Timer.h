#pragma once

#include "Clock.h"

namespace zr
{
	class Timer
	{
	public:
		Timer(const std::string& identifier);
		~Timer();

		Time Timer::restart();

	private:
		Time mStartTime;
		std::string mIdentifier;
	};
}
