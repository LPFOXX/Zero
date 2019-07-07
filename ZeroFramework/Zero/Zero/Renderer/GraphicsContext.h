#pragma once

namespace zr
{
	class GraphicsContext
	{
	public:
		GraphicsContext()
		{
		}
		virtual ~GraphicsContext()
		{
		}

		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};
}
