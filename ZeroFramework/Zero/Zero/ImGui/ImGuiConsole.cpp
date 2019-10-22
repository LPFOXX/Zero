#include <zr_pch.h>

#include "ImGuiConsole.h"

namespace zr
{
	std::shared_ptr<ImGuiConsole> Console::sImGuiConsole(new ImGuiConsole);
}