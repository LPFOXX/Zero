#include <zr_pch.h>

#include "Events.h"
#include "Application.h"
#include "EventDispatcher.h"
#include "Zero/Renderer/RenderCommand.h"
#include "Clock.h"
#include "Timer.h"

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
		dispatcher.dispatch<WindowResizeEvent>([](WindowResizeEvent& e) -> bool {
			RenderCommand::SetViewportSize(e.getWidth(), e.getHeight());
			return true;
		});

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
		using namespace std::chrono_literals;

		Clock clock(Time::minutes(20.f));
		clock.getTime().asHours();
		clock.getTime().asMicroseconds();
		clock.getTime().asMilliseconds();
		clock.getTime().asMinutes();
		clock.getTime().asNanoseconds();
		clock.getTime().asSeconds();

		Timer timer;

		Time accumulatedTime = Time::Zero();
		Time frameTime(Time::seconds(1.f / 60.f));

		while (mRunning) {
			Time elapsedTime = timer.restart();
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
			}
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}
}
