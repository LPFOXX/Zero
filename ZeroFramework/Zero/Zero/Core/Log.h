#pragma once

#include "Core.h"

#include "../../vendor/spdlog/include/spdlog/spdlog.h"
#include "../../vendor/spdlog/include/spdlog/fmt/ostr.h"

namespace zr
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger()
		{
			return sCoreLogger;
		}

		inline static Ref<spdlog::logger>& GetClientLogger()
		{
			return sClientLogger;
		}

	private:
		static Ref<spdlog::logger> sCoreLogger;
		static Ref<spdlog::logger> sClientLogger;
	};
}

#ifdef ZR_DEBUG_BUILD
#define ZR_CORE_TRACE(...)	zr::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ZR_CORE_INFO(...)	zr::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ZR_CORE_WARN(...)	zr::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ZR_CORE_ERROR(...)	zr::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ZR_CORE_FATAL(...)	zr::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define ZR_TRACE(...)	zr::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ZR_INFO(...)	zr::Log::GetClientLogger()->info(__VA_ARGS__)
#define ZR_WARN(...)	zr::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ZR_ERROR(...)	zr::Log::GetClientLogger()->error(__VA_ARGS__)
#define ZR_FATAL(...)	zr::Log::GetClientLogger()->fatal(__VA_ARGS__)

#else

#define ZR_CORE_TRACE(...)
#define ZR_CORE_INFO(...)
#define ZR_CORE_WARN(...)
#define ZR_CORE_ERROR(...)
#define ZR_CORE_FATAL(...)

#define ZR_TRACE(...)
#define ZR_INFO(...)
#define ZR_WARN(...)
#define ZR_ERROR(...)
#define ZR_FATAL(...)
#endif
