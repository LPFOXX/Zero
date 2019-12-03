#include <zr_pch.h>

#include <GLFW/glfw3.h>

#include "Zero/Core/EventDispatcher.h"
#include "Zero/Core/Application.h"
#include "Zero/Core/Profiller.h"
#include "ImGuiLayer.h"

namespace zr
{
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGui Layer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::begin()
	{
		ZR_PROFILER_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
		ZR_PROFILER_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)Application::GetWindow().getWidth(), (float)Application::GetWindow().getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::onAttach()
	{
		ZR_PROFILER_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetWindow().getNativeHandle());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, false);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiLayer::onDetach()
	{
		ZR_PROFILER_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onUpdate(const Time& elapsedTime)
	{
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool isOpen = true;
		ZR_IMGUI_DRAW("Console", &isOpen);
	}

	void ImGuiLayer::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&ImGuiLayer::onMouseButtonPressedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&ImGuiLayer::onMouseButtonReleasedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<MouseMoveEvent>(std::bind(&ImGuiLayer::onMouseMovedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrollEvent>(std::bind(&ImGuiLayer::onMouseScrolledEvent, this, std::placeholders::_1));
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&ImGuiLayer::onKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<KeyReleasedEvent>(std::bind(&ImGuiLayer::onKeyReleasedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<KeyTypedEvent>(std::bind(&ImGuiLayer::onKeyTypedEvent, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&ImGuiLayer::onWindowResizeEvent, this, std::placeholders::_1));
	}

	bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = true;
		return false;
	}

	bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = false;
		return false;
	}

	bool ImGuiLayer::onMouseMovedEvent(MouseMoveEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());
		return false;
	}

	bool ImGuiLayer::onMouseScrolledEvent(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.getYOffset();
		io.MouseWheelH += e.getXOffset();
		return false;
	}

	bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = false;
		return false;
	}

	bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(e.getKeyCode());
		return false;
	}

	bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(e.getWidth()), static_cast<float>(e.getHeight()));
		io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
		return false;
	}
}
