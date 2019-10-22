#include <zr_pch.h>

#include "Timer.h"
#include "../ImGui/ImGuiConsole.h"

namespace zr
{
	Timer::Timer(const std::string& identifier) :
		mStartTime(Time::Now()),
		mIdentifier(identifier)
	{

	}

	Timer::~Timer()
	{
		Time elapsedTime = Time::Now() - mStartTime;
		ZR_IMGUI_LOG(ConsoleItem::Trace, "%s took %f seconds (%f ms)", mIdentifier.c_str(), elapsedTime.asSeconds(), elapsedTime.asMilliseconds());
	}

	Time Timer::Timer::restart()
	{
		Time aux = Time::Now() - mStartTime;
		mStartTime = Time::Now();
		return aux;
	}
}
