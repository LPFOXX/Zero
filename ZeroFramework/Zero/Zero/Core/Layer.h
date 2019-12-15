#pragma once

#include "Events/Event.h"
#include "Clock.h"

namespace zr
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate(const Time& elapsedTime) = 0;
		virtual void onImGuiRender()
		{
		}
		virtual void onEvent(Event& e) = 0;

		inline const std::string& getName();

	private:
		std::string mName;
	};
}
