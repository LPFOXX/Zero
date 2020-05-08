#pragma once

#include "Zero/Zero/Core/Profiller.h"

extern zr::Application* zr::createApplication();

#include "../Platform/Lua/LuaBindings.hpp"

int main(int argc, char** argv)
{
	zr::Log::Init();
	ZR_PROFILER_BEGIN_SESSION("Application Startup", "application_startup.json");
	std::unique_ptr<zr::Application> app = std::unique_ptr<zr::Application>(zr::createApplication());
	ZR_PROFILER_END_SESSION();

	ZR_PROFILER_BEGIN_SESSION("Application Runtime", "application_runtime.json");
	app->run();
	ZR_PROFILER_END_SESSION();

	ZR_PROFILER_BEGIN_SESSION("Application Shutdown", "application_shutdown.json");
	app.reset();
	ZR_PROFILER_END_SESSION();
	return 0;
}

