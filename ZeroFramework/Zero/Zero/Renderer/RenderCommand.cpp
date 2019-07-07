#include <zr_pch.h>

#include "RenderCommand.h"
#include "Zero/Platform/OpenGL/OpenGLRendererAPI.h"

namespace zr
{
	std::unique_ptr<RendererAPI> RenderCommand::sRendererAPI = std::unique_ptr<RendererAPI>(new OpenGLRendererAPI);

	RenderCommand::RenderCommand()
	{

	}

	RenderCommand::~RenderCommand()
	{

	}
}
