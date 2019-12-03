#include <zr_pch.h>

#include "Events.h"
#include "EventDispatcher.h"
#include "Zero/Renderer/RenderCommand.h"
#include "Zero/Renderer/Renderer.h"
#include "Zero/Core/Log.h"
#include "Zero/Core/Profiller.h"
#include "Clock.h"
#include "Timer.h"
#include "../CommandQueue.h"

#include "Application.h"

namespace zr
{
	Application* Application::sInstance = nullptr;
	Scope<Clock> Application::sClock = nullptr;

	Application::Application()
	{
		ZR_PROFILER_FUNCTION();

		Application::sInstance = this;
		mWindow = Window::Create();
		mWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		Application::sClock = CreateScope<Clock>();

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		pushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{
		ZR_PROFILER_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::onEvent(Event& e)
	{
		ZR_PROFILER_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it) {
			(*it)->onEvent(e);
			if (e.isHandled()) {
				break;
			}
		}

		ZR_CORE_TRACE("{0}", e);
	}

	void Application::pushLayer(Layer* layer)
	{
		ZR_PROFILER_FUNCTION();

		mLayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		ZR_PROFILER_FUNCTION();

		mLayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::run()
	{
		ZR_PROFILER_FUNCTION();

		Clock clock;
		while (mRunning) {
			ZR_PROFILER_SCOPE("Run Loop");
			CommandQueue::ExecuteCommands();
			/*Time elapsedTime = timer.restart();
			accumulatedTime += elapsedTime;

			while (accumulatedTime >= frameTime) {
				accumulatedTime -= frameTime;

				for (Layer* l : mLayerStack) {
					l->onUpdate(frameTime);
				}

				mImGuiLayer->begin();
				for (Layer* layer : mLayerStack)
					layer->OnImGuiRender();
				mImGuiLayer->end();

				mWindow->onUpdate();
			}*/

			Time& elapsedTime = clock.restart();

			if (!mIsMinimized) {
				ZR_PROFILER_SCOPE("LayerStack onUpdates");

				for (Layer* l : mLayerStack) {
					l->onUpdate(elapsedTime);
				}
			}

			mImGuiLayer->begin();
			{
				ZR_PROFILER_SCOPE("Layer onImGuiRender");

				for (Layer* layer : mLayerStack)
					layer->OnImGuiRender();
				mImGuiLayer->end();
			}
			mWindow->onUpdate();
		}
	}

	void Application::CloseWindow()
	{
		ZR_PROFILER_FUNCTION();

		if (Application::sInstance != nullptr) {
			Application::sInstance->requestWindowClose();
		}
	}

	void Application::CaptureMouseCursor(bool capture)
	{
		ZR_PROFILER_FUNCTION();

		if (Application::sInstance != nullptr) {
			Application::sInstance->mWindow->captureMouseCursor(capture);
		}
	}

	void Application::requestWindowClose()
	{
		ZR_PROFILER_FUNCTION();

		mRunning = false;
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		ZR_PROFILER_FUNCTION();
		mRunning = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& event)
	{
		ZR_PROFILER_FUNCTION();

		mIsMinimized = (event.getHeight() == 0U || event.getWidth() == 0U);
		RenderCommand::SetViewportSize(event.getWidth(), event.getHeight());
		return false;
	}
}
