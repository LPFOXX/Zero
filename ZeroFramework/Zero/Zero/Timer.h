#pragma once

#include "Clock.h"

namespace zr
{
	class Timer
	{
	public:
		Timer();
		virtual ~Timer();

		const Time& getTime() const;

		Time restart();

	private:
		std::chrono::steady_clock::time_point mRestartPoint;
		mutable Time mTime;
	};
}