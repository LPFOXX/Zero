#include <pch.hpp>

#include "EditorLayer.hpp"
#include "Zero/vendor/imgui/imgui_internal.h"
#include "Zero/Zero/Core/UUID.hpp"

namespace lp
{
	EditorLayer::EditorLayer() :
		zr::Layer("EditorLayer"),
		mViewers{}
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

		mCurrentScene = zr::CreateRef<zr::Scene>();

		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());

		if (mViewers.empty())
		{
			AddSceneViewer();
		}
	}

	void EditorLayer::onDetach()
	{
		ZR_PROFILER_FUNCTION();
	}

	void EditorLayer::onUpdate(const zr::Time& elapsedTime)
	{
		mCurrentScene->onUpdate(elapsedTime);

		ZR_PROFILER_FUNCTION();

		PROFILE_SCOPE("EditorLayer::onUpdate");
		{
			PROFILE_SCOPE("Viewer::onUpdate");
			for (auto& viewer : mViewers)
			{
				viewer->onUpdate(elapsedTime);
			}
		}

		{
			ZR_PROFILER_SCOPE("Draw Viewers");
			for (size_t i = 0; i < mViewers.size(); ++i)
			{
				auto& viewer = mViewers[i];
				viewer->bind(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth, .24f, .24f, .24f, 1.f);
				{
					zr::Renderer2D::BeginScene(viewer);
					{
						auto viewerName = fmt::format("Viewer {}", i);
						ZR_PROFILER_SCOPE(viewerName.c_str());

						const auto& view = mCurrentScene->View<zr::TransformComponent, zr::SpriteComponent>();

						mCurrentScene->Each<const zr::TransformComponent, const zr::SpriteComponent>([](const zr::TransformComponent& transform, const zr::SpriteComponent& sprite) {
							zr::Renderer2D::DrawQuad(transform.getTransform(), sprite.Color);
						});

						zr::Renderer2D::EndScene();
					}
				}
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
				if (ImGui::MenuItem("Add Viewer", "ALT+V"))
				{
					AddSceneViewer();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Settings");
		{
			for (auto& result : mProfileResults) {
				auto text = fmt::format("{}: {:.3}ms", result.Name, result.Time.asMilliseconds());
				ImGui::Text(text.c_str());
			}
			mProfileResults.clear();
			ImGui::End();
		}

		std::vector<zr::Ref<zr::Viewer>> viewersToClose;
		for (size_t i = 0; i < mViewers.size(); ++i)
		{
			if (drawEditorWindow(mViewers[i], i)) {
				ZR_CORE_TRACE("Viewer {} closed.", i);
				viewersToClose.push_back(mViewers[i]);
			}
		}

		for (auto& viewer : viewersToClose)
		{
			//mViewers.erase(std::remove_if(mViewers.begin(), mViewers.end(), [&](const zr::Ref<zr::Viewer>& v) { return v == viewer; }), mViewers.end());
			mViewers.erase(std::find(mViewers.begin(), mViewers.end(), viewer));
		}
		viewersToClose.clear();

		static entt::entity selectedEntity = entt::null;
		ImGui::Begin("Scene Hierarchy");
		{
			const auto& view = mCurrentScene->View<zr::IDComponent>();
			for (auto [ent, id] : view.each())
			{
				zr::Entity entity{ mCurrentScene.get(), ent };
				auto label = fmt::format("{}##{}", entity.GetComponent<zr::TagComponent>().Tag, static_cast<uint64_t>(id.UUID));

				ImGuiTreeNodeFlags node_flags =
					ImGuiTreeNodeFlags_Leaf |
					ImGuiTreeNodeFlags_SpanAvailWidth;
				if (ImGui::TreeNodeEx(label.c_str(), node_flags))
				{
					if (ImGui::IsItemClicked()) selectedEntity = ent;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::Button("Remove Entity"))
						{
							spdlog::info("Remove Entity {}", static_cast<uint64_t>(id.UUID));
							mCurrentScene->DestroyEntity(entity);

							if (selectedEntity == ent) {
								selectedEntity = entt::null;
							}
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::TreePop();
				}
			}

			auto availableSize = ImGui::GetContentRegionAvail();
			if (ImGui::Button("Add Entity", ImVec2{ availableSize.x, 24 })) mCurrentScene->CreateEntity();

			ImGui::End();
		}

		ImGui::Begin("Properties");
		{
			if (selectedEntity != entt::null)
			{
				zr::Entity entity{ mCurrentScene.get(), selectedEntity };
				auto entityId = entity.GetComponent<zr::IDComponent>().UUID;

				auto& id = entity.GetComponent<zr::IDComponent>();
				ImGui::Text("ID: %llu", entityId);

				if (entity.HasComponent<zr::TagComponent>())
				{
					auto& tag = entity.GetComponent<zr::TagComponent>();
					ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
					static char name[128]; memset(name, 0, 128); memcpy(name, tag.Tag.c_str(), tag.Tag.size());
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 32);
					if (ImGui::InputText("##Name", name, 128)) tag.Tag = std::string(name);
					ImGui::SameLine();
					const char* availableComponents[] = { "SpriteComponent" };
					static const char* availableComponentsSelected = availableComponents[0];
					//ImGui::PushItemWidth(32);
					if (ImGui::BeginCombo("##Component", availableComponentsSelected, ImGuiComboFlags_NoPreview)) {
						for (unsigned i = 0; i < 1; ++i) {
							bool selected = availableComponentsSelected == availableComponents[i];
							if (ImGui::Selectable(availableComponents[i], selected)) {
								availableComponentsSelected = availableComponents[i];
								if (availableComponentsSelected == "SpriteComponent")
								{
									entity.AttachComponent<zr::SpriteComponent>();
								}
								else if (availableComponentsSelected == "TagComponent")
								{

								}
								else if (availableComponentsSelected == "IDComponent")
								{

								}
							}
							if (selected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					//ImGui::PopItemWidth();
				}


				static ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed;

				if (entity.HasComponent<zr::TransformComponent>()) {
					if (ImGui::CollapsingHeader("Transform", treeNodeFlags))
					{
						int flags =
							ImGuiColorEditFlags_AlphaBar |
							ImGuiColorEditFlags_AlphaPreview |
							ImGuiColorEditFlags_PickerHueBar |
							ImGuiColorEditFlags_InputRGB |
							ImGuiColorEditFlags_AlphaPreviewHalf;
						auto& transformable = entity.GetComponent<zr::TransformComponent>();

						ImGui::DragFloat3("Translation", glm::value_ptr(transformable.Position), .1f);
						ImGui::DragFloat3("Rotation", glm::value_ptr(transformable.Rotation));
						ImGui::DragFloat3("Scale", glm::value_ptr(transformable.Scale), .1f, 0.f, std::numeric_limits<float>::max());
					}
				}

				if (entity.HasComponent<zr::SpriteComponent>()) {
					if (ImGui::CollapsingHeader("Sprite", treeNodeFlags))
					{
						int flags =
							ImGuiColorEditFlags_AlphaBar |
							ImGuiColorEditFlags_AlphaPreview |
							ImGuiColorEditFlags_PickerHueBar |
							ImGuiColorEditFlags_InputRGB |
							ImGuiColorEditFlags_AlphaPreviewHalf;
						auto& sprite = entity.GetComponent<zr::SpriteComponent>();
						ImGui::ColorEdit4("Color", &sprite.Color[0], flags);
					}
				}

				ImGui::Separator();
			}
			else
			{
				ImGui::Text("No entity selected.");
			}
			ImGui::End();
		}
	}

	bool EditorLayer::drawEditorWindow(zr::Ref<zr::Viewer>& viewer, uint32_t count)
	{
		auto& framebuffer = viewer->getFramebuffer();

		auto isOpen = true;

		unsigned windowFlags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		auto viewLabel = fmt::format("View##edit_view{}", count);
		if (!ImGui::Begin(viewLabel.c_str(), &isOpen, windowFlags)) {
			ImGui::End();
			ImGui::PopStyleVar();
			return false;
		}

		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Menu")) {
				if (ImGui::BeginMenu("Options")) {
					static bool enableRotation = false;
					if (ImGui::Checkbox("Enable Z Rotation", &enableRotation)) {
						viewer->getCameraController()->enableRotation(enableRotation);
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

					static int maxMSAALevel = zr::Framebuffer::GetMaxSamples();
					static int msaaLevel = framebuffer->getProperties().getMSAASamples() == 0 ? 0 : (int)std::log2f((float)framebuffer->getProperties().getMSAASamples());
					if (ImGui::Combo("MSSA", &msaaLevel, "Disabled\0x2\0x4\0x8\0x16\0x32\0\0")) {
						unsigned sampleCount = msaaLevel == 0 ? 0 : (unsigned)std::pow(2U, msaaLevel - 1);
						unsigned actualValueSet = framebuffer->setMSAASamples((unsigned)msaaLevel);
						if (actualValueSet != (unsigned)msaaLevel) {
							ZR_IMGUI_CONSOLE_ERROR("Can't set framebuffer multi-sample anti-aliasing level to %d. Set instead to %d.", msaaLevel, actualValueSet);
						}
					}

					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close Viewer")) isOpen = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		viewer->setEditable(ImGui::IsWindowHovered());

		const ImVec2& avail = ImGui::GetContentRegionAvail();
		auto& renderWindowSize = viewer->getRenderWindowSize();

		if (avail.x != renderWindowSize.x || avail.y != renderWindowSize.y) {
			auto event = zr::RenderWindowResizeEvent((unsigned)avail.x, (unsigned)avail.y);
			viewer->onEvent(event);
		}

		const ImVec2& pos = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddImage((void*)(uintptr_t)framebuffer->getTextureHandle(), pos, ImVec2(pos.x + avail.x, pos.y + avail.y), { 0.f, 1.f }, { 1.f, 0.f });
		ImGui::End();

		return !isOpen;
	}

	void EditorLayer::onEvent(zr::Event& e)
	{
		ZR_PROFILER_FUNCTION();

		if (e.getType() == zr::EventType::WindowResize) {
			const zr::WindowResizeEvent& resizeEvent = dynamic_cast<zr::WindowResizeEvent&>(e);
			viewportUpdate(resizeEvent.getWidth(), resizeEvent.getHeight());
		}

		if (e.getType() == zr::EventType::KeyPressed) {
			const zr::KeyPressedEvent& ev = dynamic_cast<zr::KeyPressedEvent&>(e);
			if (ev.getRepeatCount() == 0 && (zr::Keyboard)ev.getKeyCode() == zr::Keyboard::V) {
				if (zr::Input::IsKeyPressed(zr::Keyboard::LAlt))
				{
					auto& anotherViewer = mViewers.emplace_back(zr::CreateRef<zr::Viewer>());
					anotherViewer->setCameraController(new zr::OrthographicCameraController);
					anotherViewer->setFramebuffer(zr::Framebuffer::Create({ 1920, 1080, 8 }));

					anotherViewer->getInput()
						->map(zr::CameraController::Actions::MoveCameraRight, zr::Keyboard::A)
						.map(zr::CameraController::Actions::MoveCameraLeft, zr::Keyboard::D)
						.map(zr::CameraController::Actions::MoveCameraUp, zr::Keyboard::W)
						.map(zr::CameraController::Actions::MoveCameraDown, zr::Keyboard::S)
						.map(zr::CameraController::Actions::RotateCameraRight, zr::Keyboard::E)
						.map(zr::CameraController::Actions::RotateCameraLeft, zr::Keyboard::Q);
				}
			}
		}

		for (auto& viewer : mViewers)
		{
			if (viewer->isEditable())
				viewer->onEvent(e);
		}
	}

	void EditorLayer::onViewportUpdate(const glm::vec2& viewportSize)
	{
		ZR_PROFILER_FUNCTION();
		ZR_INFO("[EditorLayer] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		notify(viewportSize);
	}

	void EditorLayer::AddSceneViewer()
	{
		auto& anotherViewer = mViewers.emplace_back(zr::CreateRef<zr::Viewer>());
		anotherViewer->setCameraController(new zr::OrthographicCameraController);
		anotherViewer->setFramebuffer(zr::Framebuffer::Create({ 1920, 1080, 8 }));

		anotherViewer->getInput()
			->map(zr::CameraController::Actions::MoveCameraRight, zr::Keyboard::A)
			.map(zr::CameraController::Actions::MoveCameraLeft, zr::Keyboard::D)
			.map(zr::CameraController::Actions::MoveCameraUp, zr::Keyboard::W)
			.map(zr::CameraController::Actions::MoveCameraDown, zr::Keyboard::S)
			.map(zr::CameraController::Actions::RotateCameraRight, zr::Keyboard::E)
			.map(zr::CameraController::Actions::RotateCameraLeft, zr::Keyboard::Q);
	}
}
