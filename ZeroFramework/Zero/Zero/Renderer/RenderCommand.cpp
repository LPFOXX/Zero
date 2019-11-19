#include <zr_pch.h>

#include "RenderCommand.h"
#include "Zero/Platform/OpenGL/OpenGLRendererAPI.h"

namespace zr
{
	Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRendererAPI>();

	RenderCommand::RenderCommand()
	{

	}

	RenderCommand::~RenderCommand()
	{

	}
}
