#pragma once

#include "Clock.h"

namespace zr
{
	struct ProfileResult
	{
		ProfileResult() :
			Name(""),
			Time(zr::Time::Zero())
		{

		}

		ProfileResult(const char* name, const zr::Time& time) :
			Name(name),
			Time(time)
		{

		}

		const char* Name;
		zr::Time Time;
	};

	template <typename Function>
	class Timer
	{
	public:
		Timer(const char* identifier, Function&& func);
		~Timer();

		Time restart();

	private:
		Time mStartTime;
		Function mFunc;
		const char* mIdentifier;
	};
}

#include "Timer.inl"

