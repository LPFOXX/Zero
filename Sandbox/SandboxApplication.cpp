#include <pch.h>

#include "SandboxLayer.h"
#include "HUDLayer.h"
#include "SandboxApplication.h"

#include "Zero/Zero/Core/EntryPoint.h"

namespace lp
{
	SandboxApplication::SandboxApplication() :
		zr::Application()
	{
		zr::Profiller::Get().beginSession("Application Startup", "application_startup.json");
		{
			PROFILER_SCOPE("Application Startup");
			//pushLayer(new SandboxLayer);
			pushLayer(new HUDLayer);
		}
		zr::Profiller::Get().endSession();
	}

	SandboxApplication::~SandboxApplication()
	{
		zr::Profiller::Get().beginSession("Application Shutdown", "application_shutdown.json");
		{
			PROFILER_SCOPE("Application Shutdown");
		}
		zr::Profiller::Get().endSession();
	}
}

zr::Application* zr::createApplication()
{
	return new lp::SandboxApplication;
}