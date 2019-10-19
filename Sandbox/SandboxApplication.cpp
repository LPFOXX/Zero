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
		//pushLayer(new SandboxLayer);
		pushLayer(new HUDLayer);
	}

	SandboxApplication::~SandboxApplication()
	{

	}
}

zr::Application* zr::createApplication()
{
	return new lp::SandboxApplication;
}