#pragma once

#include "Events.h"
#include "ImGui/ImGuiLayer.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

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

		inline static Application& getInstance()
		{
			return *sInstance;
		}

		inline Window& getWindow() const
		{
			return *mWindow;
		}

	protected:
		bool onWindowClose(WindowCloseEvent& event);

	private:
		static Application* sInstance;

	private:
		std::unique_ptr<Window> mWindow;
		LayerStack mLayerStack;
		ImGuiLayer* mImGuiLayer;

		std::shared_ptr<VertexArray> mVertexArray;
		std::shared_ptr<Shader> mShader;

		std::shared_ptr<Shader> mBlueShader;
		std::shared_ptr<VertexArray> mSquareVA;

		bool mRunning = true;
	};

	Application* createApplication();
}
