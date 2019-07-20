#include <zr_pch.h>

#include "Log.h"

#include "../vendor/spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace zr
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v %$");
		Log::sCoreLogger = spdlog::stdout_color_mt("ZeRo");
		Log::sCoreLogger->set_level(spdlog::level::trace);
		Log::sClientLogger = spdlog::stdout_color_mt("Client App");
		Log::sClientLogger->set_level(spdlog::level::trace);
	}

}
