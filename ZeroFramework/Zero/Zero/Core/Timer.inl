#include "../../zr_pch.h"

#include "../ImGui/ImGuiConsole.h"

namespace zr
{
	template <typename Function>
	inline Timer<Function>::Timer(const char* identifier, Function&& func) :
		mStartTime(Time::Now()),
		mFunc(func),
		mIdentifier(identifier)
	{

	}

	template <typename Function>
	inline Timer<Function>::~Timer()
	{
		Time elapsedTime = Time::Now() - mStartTime;
		//ZR_IMGUI_LOG(ConsoleItem::Trace, "%s took %f seconds (%f ms)", mIdentifier, elapsedTime.asSeconds(), elapsedTime.asMilliseconds());
		mFunc({mIdentifier, elapsedTime});
	}

	template <typename Function>
	inline Time Timer<Function>::Timer::restart()
	{
		Time aux = Time::Now() - mStartTime;
		mStartTime = Time::Now();
		return aux;
	}
}
