#pragma once

#include <chrono>

namespace zr
{
	class Time
	{
	public:
		using seconds =			std::chrono::duration<float, std::ratio<1>>;
		using milliseconds =	std::chrono::duration<float, std::ratio<1, 1000>>;
		using microseconds =	std::chrono::duration<float, std::ratio<1, 1000000>>;
		using nanoseconds =		std::chrono::duration<float, std::ratio<1, 1000000000>>;
		using minutes =			std::chrono::duration<float, std::ratio<60>>;
		using hours =			std::chrono::duration<float, std::ratio<3600>>;

	public:
		Time();
		Time(float timeInSeconds);
		Time(const Time::seconds& time);
		virtual ~Time();

		const Time& operator+=(const Time& rhs)
		{
			mTime += rhs.mTime;
			return *this;
		}

		const Time& operator-=(const Time& rhs)
		{
			mTime -= rhs.mTime;
			return *this;
		}

		bool operator>=(Time& rhs)
		{
			return mTime >= rhs.mTime;
		}

	public:
		static Time Zero();
		static Time Now();

	public:
		float asSeconds() const
		{
			return mTime.count();
		}

		float asMilliseconds() const
		{
			return milliseconds(mTime).count();
		}

		float asMicroseconds() const
		{
			return microseconds(mTime).count();
		}

		float asNanoseconds() const
		{
			return nanoseconds(mTime).count();
		}

		float asMinutes() const
		{
			return std::chrono::duration_cast<minutes>(mTime).count();
		}

		float asHours() const
		{
			return std::chrono::duration_cast<hours>(mTime).count();
		}

	private:
		seconds mTime;
	};

	class Clock
	{
	public:
		Clock();
		Clock(const Time& time);
		Clock(const Time::seconds& seconds);
		Clock(float seconds);
		virtual ~Clock();

		inline const Time& getTime() const
		{
			return mElapsedTime;
		};

	private:
		Time mElapsedTime;
	};
}
