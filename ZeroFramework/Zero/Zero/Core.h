#pragma once

namespace zr
{
	/**
	* @brief Helper funtion to clamp values in a certain interval.
	*
	* It ensures that a given value stays in the interval [min-max].
	* If it is less than min the value returned is min and if it is more than max
	* the value returned is max.
	*
	* @return a values within the interval [min-max].
	*
	* @todo Change this function into a template function.
	*
	*/
	template <typename T, typename U>
	T clamp(T min, T max, U value)
	{
		if (static_cast<T>(value) >= max) {
			return max;
		}

		if (static_cast<T>(value) <= 0) {
			return min;
		}

		return static_cast<T>(value);
	}
}
