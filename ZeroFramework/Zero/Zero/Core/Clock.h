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
		virtual ~Time();

		static Time Zero()
		{
			return Time(0LL);
		}

		static Time Now()
		{
			return Time(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count());
		}

		static Time Hours(float hours)
		{
			return Time(static_cast<long long>(hours) * 3600LL * 1000000LL);
		}

		static Time Minutes(float minutes)
		{
			return Time(static_cast<long long>(minutes) * 60LL * 1000000LL);
		}

		static Time Seconds(float seconds)
		{
			return Time(static_cast<long long>(seconds) * 1000000LL);
		}

		static Time Milliseconds(float milliseconds)
		{
			return Time(static_cast<long long>(milliseconds) * 1000LL);
		}

		static Time Microseconds(float microseconds)
		{
			return Time((long long)microseconds);
		}

		operator float()
		{
			return (float) mTimeInMicroseconds;
		}

		const Time& operator+=(const Time& rhs)
		{
			mTimeInMicroseconds += rhs.mTimeInMicroseconds;
			return *this;
		}

		Time& operator-=(const Time& rhs)
		{
			mTimeInMicroseconds -= rhs.mTimeInMicroseconds;
			return *this;
		}

		Time operator-(const Time& rhs) const 
		{
			Time time;
			time.mTimeInMicroseconds = mTimeInMicroseconds - rhs.mTimeInMicroseconds;
			return time;
		}

		Time& operator=(const Time& rhs)
		{
			mTimeInMicroseconds = rhs.mTimeInMicroseconds;
			return *this;
		}

		bool operator>=(const Time& rhs) const
		{
			return mTimeInMicroseconds >= rhs.mTimeInMicroseconds;
		}

		bool operator<(const Time& rhs) const
		{
			return mTimeInMicroseconds < rhs.mTimeInMicroseconds;
		}

		bool operator==(const Time& rhs) const
		{
			return mTimeInMicroseconds == rhs.mTimeInMicroseconds;
		}

	public:
		inline float asHours() const
		{
			return asMinutes() * (1.f / 60.f);
		}

		inline float asMinutes() const
		{
			return asSeconds() * (1.f / 60.f);
		}

		inline float asSeconds() const
		{
			return 0.000001f * mTimeInMicroseconds;
		}

		inline float asMilliseconds() const
		{
			return 0.001f * mTimeInMicroseconds;
		}

		inline float asMicroseconds() const
		{
			return static_cast<float>(mTimeInMicroseconds);
		}

		inline float asNanoseconds() const
		{
			return mTimeInMicroseconds * 1000.f;
		}

	private:
		Time(long long microseconds) :
			mTimeInMicroseconds(microseconds)
		{

		}

	private:
		long long mTimeInMicroseconds;
	};

	class Clock
	{
	public:
		Clock();
		virtual ~Clock();

		inline Time getElapsedTime() const
		{
			return (Time::Now() - mLastRestartPoint);
		};

		Time restart()
		{
			Time& now = Time::Now();
			Time elapsedTime = now - mLastRestartPoint;
			mLastRestartPoint = now;
			return elapsedTime;
		}

	private:
		mutable Time mLastRestartPoint;
	};
}
