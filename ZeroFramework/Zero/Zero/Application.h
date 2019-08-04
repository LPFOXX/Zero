#pragma once

#include "Events.h"
#include "ImGui/ImGuiLayer.h"
#include "Window.h"
#include "LayerStack.h"
#include "Timer.h"

namespace zr
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void onEvent(Event& e);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		void run();

		inline static Application& GetInstance()
		{
			return *sInstance;
		}

		inline Window& getWindow() const
		{
			return *mWindow;
		}

	public:
		static void CloseWindow();
		static void CaptureMouseCursor(bool capture = true);

	private:
		void requestWindowClose();

	public:
		static const Time& GetTime()
		{
			return sTimer->getTime();
		}

	protected:
		bool onWindowClose(WindowCloseEvent& event);

	private:
		static Application* sInstance;
		static std::unique_ptr<Timer> sTimer;

	private:
		std::unique_ptr<Window> mWindow;
		LayerStack mLayerStack;
		ImGuiLayer* mImGuiLayer;

		bool mRunning = true;
	};

	Application* createApplication();
}
