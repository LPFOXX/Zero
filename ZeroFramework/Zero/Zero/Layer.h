#pragma once

#include "Events/Event.h"

namespace zr
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void onEvent(Event& e) = 0;

		inline const std::string& getName();

	private:
		std::string mName;
	};
}
