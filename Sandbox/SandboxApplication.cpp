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
		ZR_PROFILER_FUNCTION();
		//pushLayer(new SandboxLayer);
		pushLayer(new HUDLayer);
	}

	SandboxApplication::~SandboxApplication()
	{
		ZR_PROFILER_FUNCTION();
	}
}

zr::Application* zr::createApplication()
{
	return new lp::SandboxApplication;
}
