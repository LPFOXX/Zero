#pragma once

#include "../vendor/spdlog/include/spdlog/spdlog.h"
#include "../vendor/spdlog/include/spdlog/fmt/ostr.h"

namespace zr
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return sCoreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return sClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

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