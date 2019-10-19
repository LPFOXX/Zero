#include <zr_pch.h>

#include "Events.h"
#include "EventDispatcher.h"
#include "Zero/Renderer/RenderCommand.h"
#include "Zero/Renderer/Renderer.h"
#include "Zero/Core/Log.h"
#include "Clock.h"
#include "Timer.h"
#include "../CommandQueue.h"

#include "Application.h"

namespace zr
{
	Application* Application::sInstance = nullptr;
	std::unique_ptr<Clock> Application::sClock = nullptr;

	Application::Application()
	{
		mWindow.reset(Window::Create());
		Application::sInstance = this;
		Application::sClock.reset(new Clock);

		mWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		pushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::onEvent(Event& e)
	{
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
		mLayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		mLayerStack.pushOverlay(layer);
	}

	void Application::run()
	{
		Clock clock;

		while (mRunning) {
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
				for (Layer* l : mLayerStack) {
					l->onUpdate(elapsedTime);
				}
			}

			mImGuiLayer->begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->end();

			mWindow->onUpdate();
		}
	}

	void Application::CloseWindow()
	{
		if (Application::sInstance != nullptr) {
			Application::sInstance->requestWindowClose();
		}
	}

	void Application::CaptureMouseCursor(bool capture)
	{
		if (Application::sInstance != nullptr) {
			Application::sInstance->mWindow->captureMouseCursor(capture);
		}
	}

	void Application::requestWindowClose()
	{
		mRunning = false;
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& event)
	{
		mIsMinimized = (event.getHeight() == 0U || event.getWidth() == 0U);
		RenderCommand::SetViewportSize(event.getWidth(), event.getHeight());
		return false;
	}
}
