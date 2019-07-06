#include <zr_pch.h>

#include "Events.h"
#include "Application.h"
#include "EventDispatcher.h"
#include "Input.h"

namespace zr
{
	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		mWindow.reset(Window::Create());
		Application::sInstance = this;
		mWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

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

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it) {
			(*it)->onEvent(e);
			if (e.isHandled()) {
				break;
			}
		}

		std::cout << e.toString() << '\n';
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
		while (mRunning) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* l : mLayerStack) {
				l->onUpdate();
			}

			mImGuiLayer->begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->end();

			mWindow->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}
}
