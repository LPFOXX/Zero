#pragma once

#include "Clock.h"

namespace zr
{
	class Timer
	{
	public:
		Timer();
		virtual ~Timer();

		Time restart();

	private:
		std::chrono::steady_clock::time_point mRestartPoint;
		Time mTime;
	};
}