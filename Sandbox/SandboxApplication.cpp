#include <pch.h>

#include "SandboxLayer.h"
#include "SandboxApplication.h"

#include "Zero/Zero/EntryPoint.h"

namespace lp
{
	SandboxApplication::SandboxApplication() :
		zr::Application()
	{
		pushLayer(new SandboxLayer);
	}

	SandboxApplication::~SandboxApplication()
	{

	}
}

zr::Application* zr::createApplication()
{
	return new lp::SandboxApplication;
}