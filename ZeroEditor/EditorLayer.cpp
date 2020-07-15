#include <pch.hpp>

#include "EditorLayer.hpp"
#include "Zero/vendor/imgui/imgui_internal.h"

namespace lp
{
	EditorLayer::EditorLayer() :
		zr::Layer("EditorLayer"),
		mViewer(nullptr)
	{
		ZR_PROFILER_FUNCTION();
	}

	EditorLayer::~EditorLayer()
	{
		ZR_PROFILER_FUNCTION();
	}

	void EditorLayer::onAttach()
	{
		ZR_PROFILER_FUNCTION();

		mViewer = zr::CreateRef<zr::Viewer>();
		mViewer->setCameraController(new zr::OrthographicCameraController);
		mViewer->setFramebuffer(zr::Framebuffer::Create({ 1920, 1080, 8 }));

		mViewer->getInput()
			->map(zr::CameraController::Actions::MoveCameraRight, zr::Keyboard::A)
			.map(zr::CameraController::Actions::MoveCameraLeft, zr::Keyboard::D)
			.map(zr::CameraController::Actions::MoveCameraUp, zr::Keyboard::W)
			.map(zr::CameraController::Actions::MoveCameraDown, zr::Keyboard::S)
			.map(zr::CameraController::Actions::RotateCameraRight, zr::Keyboard::E)
			.map(zr::CameraController::Actions::RotateCameraLeft, zr::Keyboard::Q);

		float windowWidth = (float)zr::Application::GetWindow().getWidth();
		float windowHeight = (float)zr::Application::GetWindow().getHeight();

		const auto& handler1 = zr::AssetManager<zr::Font>::Get().load("resources/fonts/futura_book_font.ttf");
		const auto& handler2 = zr::AssetManager<zr::Texture2D>::Get().load("resources/textures/Checkerboard.png");
		const auto& handler3 = zr::AssetManager<zr::Shader>::Get().load("resources/shaders/Texture.glsl");
		//const auto& handler4 = zr::AssetManager<zr::Model3D>::Get().load("resources/models/nanosuit.obj");

		//const auto& handler5 = zr::AssetManager<zr::Framebuffer>::Get().create("MainFramebuffer", zr::Framebuffer::Properties{ 1280, 720, 8 });

		if (!mLuaVM.loadFile("resources/scripts/Layer.lua")) {
			ZR_IMGUI_CONSOLE_ERROR("Unable to load Lua script file.");
		}

		if (!mLuaVM.runFunction("onAttach", 0)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua funtion.");
		}

		mQuad.setPosition(0.f, 0.f);
		//mQuad.setColor({ 1.f, 1.f, 0.f, 1.f });
		mQuad.setSize(50.f, 50.f);
		mQuad.setColor(
			{ 1.f, 0.f, 0.f, 1.f },		// Red
			{ 0.f, 1.f, 0.f, 1.f },		// Green
			{ 0.f, 0.f, 1.f, 1.f },		// Blue
			{ 1.f, 1.f, 0.f, 1.f });	// Yellow

		mSprite2D.setPosition(0.f, 0.f);
		mSprite2D.setSize(25.f, 25.f);
		mQuad.setSize(25.f, 25.f);

		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());
	}

	void EditorLayer::onDetach()
	{
		ZR_PROFILER_FUNCTION();

		if (!mLuaVM.runFunction("onDetach", 0)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}
	}

	void EditorLayer::onUpdate(const zr::Time& elapsedTime)
	{
		ZR_PROFILER_FUNCTION();
		if (!mLuaVM.runFunction("onUpdate", 0, elapsedTime)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}

		PROFILE_SCOPE("EditorLayer::onUpdate");
		{
			PROFILE_SCOPE("Viewer::onUpdate");
			mViewer->onUpdate(elapsedTime);
		}

		mViewer->bind(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth, .3f, .3f, .8f, 1.f);
		{
			ZR_PROFILER_SCOPE("Renderer Draw");
			const zr::Time& time = zr::Application::GetTime();

			zr::Renderer2D::BeginScene(mViewer);
			{
				{
					PROFILE_SCOPE("Draw 2 Quads");
					zr::Renderer2D::DrawRotatedQuad({ 0.5f, -.5f }, { .5f, .75f }, glm::radians(-time.asSeconds() * 30.f), { .2f, .3f, .8f, 1.f });
					zr::Renderer2D::DrawRotatedQuad({ -1.f, 0.f }, { .8f, .8f }, glm::radians(time.asSeconds() * 45.f), { .8f, .2f, .3f, 1.f });
				}

				zr::Renderer2D::EndScene();
			}
		}

		zr::Window& window = zr::Application::GetWindow();
		zr::Framebuffer::BindDefault();
		zr::RenderCommand::SetViewportSize(window.getWidth(), window.getHeight());
		zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color);
		zr::RenderCommand::SetClearColor(.3f, .3f, 0.3f, 1.f);
	}

	void EditorLayer::onImGuiRender()
	{
		ZR_PROFILER_FUNCTION();

		static bool showViewer = true;
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Show Viewer", nullptr, &showViewer);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Settings");
		{
			for (auto& result : mProfileResults) {
				char label[50];
				strcpy(label, "%.3fms ");
				strcat(label, result.Name);
				ImGui::Text(label, result.Time.asMilliseconds());
			}
			mProfileResults.clear();

			static float quadDepthLevel = 0.f;
			if (ImGui::DragFloat("Quad Depth Level", &quadDepthLevel, 0.01f)) {
				mQuad.setDepthLevel(quadDepthLevel);
			}

			if (showViewer) drawEditorWindow(mViewer, showViewer);

			ImGui::End();
		}
	}

	void EditorLayer::drawEditorWindow(zr::Ref<zr::Viewer>& viewer, bool& open)
	{
		auto& framebuffer = viewer->getFramebuffer();

		unsigned windowFlags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		if (!ImGui::Begin("View##edit_view", &open, windowFlags)) {
			ImGui::End();
			ImGui::PopStyleVar();
			return;
		}

		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Menu")) {
				if (ImGui::BeginMenu("Options")) {
					static bool enableRotation = false;
					if (ImGui::Checkbox("Enable Z Rotation", &enableRotation)) {
						mViewer->getCameraController()->enableRotation(true);
					}

					const char* cameraOptions[] = { "Orthographic Camera", "Perspective Camera" };
					static const char* cameraOptionSelected = cameraOptions[0];
					if (ImGui::BeginCombo("Camera", cameraOptionSelected)) {
						for (unsigned i = 0; i < 2; ++i) {
							bool selected = cameraOptionSelected == cameraOptions[i];
							if (ImGui::Selectable(cameraOptions[i], selected)) cameraOptionSelected = cameraOptions[i];
							if (selected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::Separator();

					static int framebufferSize[] = { (int)framebuffer->getProperties().Width, (int)framebuffer->getProperties().Height };
					static std::pair<unsigned, unsigned> framebufferLimits = zr::Framebuffer::GetMaxViewportSize();
					if (ImGui::DragInt("Framebuffer Width", &framebufferSize[0], 1.f, 1, framebufferLimits.first)) {
						framebuffer->setSize((unsigned)framebufferSize[0], (unsigned)framebufferSize[1]);
					}

					if (ImGui::DragInt("Framebuffer Height", &framebufferSize[1], 1.f, 1, framebufferLimits.second)) {
						framebuffer->setSize((unsigned)framebufferSize[0], (unsigned)framebufferSize[1]);
					}

					static int msaaLevel = framebuffer->getProperties().getMSAASamples() == 0 ? 0 : (int)std::log2f((float)framebuffer->getProperties().getMSAASamples());
					if (ImGui::Combo("MSSA", &msaaLevel, "Disabled\0x2\0x4\0x8\0x16\0\0")) {
						unsigned sampleCount = msaaLevel == 0 ? 0 : (unsigned)std::pow(2U, msaaLevel - 1);
						unsigned actualValueSet = framebuffer->setMSAASamples((unsigned)msaaLevel);
						if (actualValueSet != (unsigned)msaaLevel) {
							ZR_IMGUI_CONSOLE_ERROR("Can't set framebuffer multi-sample anti-aliasing level to %d. Set instead to %d.", msaaLevel, actualValueSet);
						}
					}

					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close Window")) open = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		viewer->setEditable(ImGui::IsWindowHovered());

		const ImVec2& avail = ImGui::GetContentRegionAvail();
		auto& renderWindowSize = viewer->getRenderWindowSize();

		if (avail.x != renderWindowSize.x || avail.y != renderWindowSize.y) {
			viewer->onEvent(zr::RenderWindowResizeEvent((unsigned)avail.x, (unsigned)avail.y));
		}

		const ImVec2& pos = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddImage((void*)(uintptr_t)framebuffer->getTextureHandle(), pos, ImVec2(pos.x + avail.x, pos.y + avail.y), { 0.f, 1.f }, { 1.f, 0.f });
		ImGui::End();
	}

	void EditorLayer::onEvent(zr::Event& e)
	{
		ZR_PROFILER_FUNCTION();
		if (!mLuaVM.runFunction("onEvent", 0, e)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}

		if (e.getType() == zr::EventType::WindowResize) {
			const zr::WindowResizeEvent& resizeEvent = dynamic_cast<zr::WindowResizeEvent&>(e);
			viewportUpdate(resizeEvent.getWidth(), resizeEvent.getHeight());
		}

		if (mViewer->isEditable())
			mViewer->onEvent(e);
	}

	void EditorLayer::onViewportUpdate(const glm::vec2& viewportSize)
	{
		ZR_PROFILER_FUNCTION();
		ZR_INFO("[EditorLayer] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		notify(viewportSize);
	}
}
