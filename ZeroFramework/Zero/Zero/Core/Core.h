#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define HZ_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define ZR_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define HZ_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define ZR_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define ZR_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

#ifdef NDEBUG
// Release
#define ZR_RELEASE_BUILD
#else
// Debug
#define ZR_DEBUG_BUILD
#define ZR_ENABLE_ASSERTS
#endif

#define PROFILING

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
	*/
	template <typename T, typename U>
	constexpr T clamp(T min, T max, U value)
	{
		if (static_cast<T>(value) >= max) {
			return max;
		}

		if (static_cast<T>(value) <= 0) {
			return min;
		}

		return static_cast<T>(value);
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
