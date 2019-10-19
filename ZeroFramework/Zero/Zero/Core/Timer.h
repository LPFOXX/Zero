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
		Time mRestartTimePoint;
	};
}