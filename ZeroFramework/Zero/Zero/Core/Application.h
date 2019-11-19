#pragma once

#include "Events.h"
#include "../ImGui/ImGuiLayer.h"
#include "Window.h"
#include "LayerStack.h"
#include "Timer.h"
#include "Core.h"

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

		inline static Window& GetWindow()
		{
			return *Application::sInstance->mWindow;
		}

	public:
		static void CloseWindow();
		static void CaptureMouseCursor(bool capture = true);

	private:
		void requestWindowClose();

	public:
		static const Time GetTime()
		{
			return sClock->getElapsedTime();
		}

	protected:
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResize(WindowResizeEvent& event);

	private:
		static Application* sInstance;
		static Scope<Clock> sClock;

	private:
		Scope<Window> mWindow;
		LayerStack mLayerStack;
		ImGuiLayer* mImGuiLayer;

		bool mRunning = true;
		bool mIsMinimized = false;
	};

	Application* createApplication();
}
