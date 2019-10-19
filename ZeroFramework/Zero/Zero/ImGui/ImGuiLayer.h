#pragma once

#include "../Core/Layer.h"
#include "../Core/Clock.h"

namespace zr
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void begin();
		void end();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const Time& elapsedTime) override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(Event& e) override;

	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMoveEvent& e);
		bool onMouseScrolledEvent(MouseScrollEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);

	private:
		float mTime = 0.0f;
	};
}
