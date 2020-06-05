#include <pch.h>

#include "SandboxLayer.h"
#include <Zero/Zero.h>

#include "Zero/vendor/glm/include/glm/gtc/matrix_transform.hpp"

namespace lp
{
	//zr::ImGuiConsole SandboxLayer::sImguiConsole;

	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer"),
		mOrthographicCameraController(nullptr),
		mPerspectiveCameraController(nullptr),
		mFramebuffer(nullptr),
		mFPSCamera(nullptr)
	{
		zr::ModelLoader::Get()->setProgressHandler(std::make_shared<zr::ModelProgressHandler>());
		zr::ModelLoader::Get()->setLogger(std::make_shared<zr::ModelLogger>(Assimp::Logger::LogSeverity::VERBOSE));

		mOrthographicCameraController = std::make_shared<zr::OrthographicCameraController>(1280.f / 600.f, true);
		mPerspectiveCameraController = std::make_shared<zr::PerspectiveCameraController>(1280.f / 600.f, true);
		mFPSCamera = std::make_shared<zr::FPSCamera>(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), 1280.f, 600.f);
		mFPSCamera->invertMouseMovement(!mIsMouseCaptured);

		unsigned componentsToLoad =
			zr::MeshData::Normals |
			zr::MeshData::TangentsAndBitangents |
			zr::MeshData::TextureCoordinates |
			zr::MeshData::Textures |
			zr::MeshData::Animations |
			zr::MeshData::Materials;
		std::vector<std::string> modelsToLoad{ {
				//{"resources/nanosuit/nanosuit.obj"},
				//{"resources/anim_model/anim_model.dae"},
				{"resources/iron_man_fixed/iron_man_fixed.obj"},
				//{"resources/bob_lamp/boblampclean.md5mesh"},
				//{"resources/teapot3/teapot.obj"}
				} };

		for (unsigned i = 0; i < modelsToLoad.size(); ++i) {
			mModels.push_back(std::make_shared<zr::Model>(modelsToLoad[i], componentsToLoad, [&](float& progress) {
				ZR_INFO("Progress: {0:.2f}", progress);
			}));
		}

		/*for (auto& model : mModels) {
			model->setAnimation(0, true);
		}*/
		/*mModel = std::make_shared<zr::Model>("resources/nanosuit/nanosuit.obj", componentsToLoad, [&](float& progress) {
			mProgress = progress;
		});*/
		/*mModel = std::make_shared<zr::Model>("resources/anim_model/anim_model.dae", componentsToLoad, [&](float& progress) {
			mProgress = progress;
		});*/
		/*mModel = std::make_shared<zr::Model>("resources/iron_man_fixed/iron_man_fixed.obj",componentsToLoad, [&](float& progress) {
			mProgress = progress;
		});*/
		/*mModel = std::make_shared<zr::Model>("resources/teapot3/teapot.obj",componentsToLoad, [&](float& progress) {
			mProgress = progress;
		});*/


		mCubeMap = zr::CubeMap::Create();
		bool success = mCubeMap->loadFromFiles({
			"resources/skybox/right.jpg",
			"resources/skybox/left.jpg",
			"resources/skybox/top.jpg",
			"resources/skybox/bottom.jpg",
			"resources/skybox/back.jpg",
			"resources/skybox/front.jpg"
			});

		if (!success) {
			ZR_ERROR("CubeMap loading error.");
		}

		mFramebuffer = zr::Framebuffer::Create({1280, 600, 8});

		float vertices[3 * 7]{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		unsigned indices[3]{
			0, 1, 2
		};

		mVertexArray = zr::VertexArray::Create();

		std::shared_ptr<zr::VertexBuffer> vertexBuffer = zr::VertexBuffer::Create(vertices, sizeof(vertices), zr::DrawMode::Static);
		zr::BufferLayout layout = {
			{ zr::ShaderDataType::Float3, "aPosition" },
			{ zr::ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->setLayout(layout);
		mVertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<zr::IndexBuffer> indexBuffer = zr::IndexBuffer::Create(indices, 3U, zr::DrawMode::Static);
		mVertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		mSquareVA = zr::VertexArray::Create();

		std::shared_ptr<zr::VertexBuffer> squareVB = zr::VertexBuffer::Create(squareVertices, sizeof(squareVertices), zr::DrawMode::Static);
		squareVB->setLayout({
			{ zr::ShaderDataType::Float3, "aPosition" }
			});
		mSquareVA->addVertexBuffer(squareVB);

		unsigned squareIndices[6]{
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<zr::IndexBuffer> squareIB = zr::IndexBuffer::Create(squareIndices, 6U, zr::DrawMode::Static);
		mSquareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			uniform mat4 uViewProjection;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = uViewProjection * vec4(aPosition, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 vPosition;

			void main()
			{
				color = vec4(vPosition * 0.5 + 0.5, 1.0);
			}
		)";

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			out vec3 vPosition;

			void main()
			{
				vPosition = aPosition;
				gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 vPosition;
			uniform vec4 uColor;

			void main()
			{
				color = uColor;
			}
		)";

		mShader = zr::Shader::Create();
		if (!mShader->loadFromStrings("SandboxLayerShader", vertexSrc, fragmentSrc)) {
			ZR_CORE_ERROR("Error creating Shader object!");
		}

		mBlueShader = zr::Shader::Create();
		if (!mBlueShader->loadFromStrings("SandboxLayerBlueShader", blueShaderVertexSrc, blueShaderFragmentSrc)) {
			ZR_CORE_ERROR("Error creating blue Shader object!");
		}
	}

	SandboxLayer::~SandboxLayer()
	{
	}

	void SandboxLayer::onAttach()
	{
		ZR_PROFILER_FUNCTION();

	}

	void SandboxLayer::onDetach()
	{
	}

	void SandboxLayer::onUpdate(const zr::Time& elapsedTime)
	{
		mOrthographicCameraController->onUpdate(elapsedTime);
		mPerspectiveCameraController->onUpdate(elapsedTime);

		for (auto& model : mModels) {
			model->update(elapsedTime);
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::I)) {
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Forward, elapsedTime);
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::K)) {
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Backward, elapsedTime);
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::J)) {
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Left, elapsedTime);
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::L)) {
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Right, elapsedTime);
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::U)) {
			mFPSCamera->rotate(elapsedTime.asSeconds());
		}

		if (zr::Input::IsKeyPressed(zr::Keyboard::P)) {
			mFPSCamera->rotate(-elapsedTime.asSeconds());
		}

		zr::Renderer::BeginScene(mFPSCamera);
		{
			zr::RenderCommand::SetClearColor(.2f, .2f, .2f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);
			zr::RenderCommand::EnableDepthTest(true);
			zr::RenderCommand::EnableFaceCulling(true);

			zr::Renderer::Submit(mCubeMap, true);

			for (unsigned i = 0; i < mModels.size(); ++i) {
				glm::mat4& model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(10.f * i, 0.f, 0.f));
				model = glm::scale(model, glm::vec3(mModelScaleFactor, mModelScaleFactor, mModelScaleFactor));
				mModels[i]->setModelTransform(model);
				mModels[i]->render(mFPSCamera->getViewProjectionMatrix());
				//mModels[i]->render(mOrthographicCameraController->getCamera()->getViewProjectionMatrix());
			}

			zr::Renderer::EndScene();
		}

		/*zr::Renderer::BeginScene(mOrthographicCameraController->getCamera());
		{
			zr::RenderCommand::SetClearColor(1.f, 0.f, 1.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			glm::vec4 blueColor(.2f, .3f, .8f, 1.f);
			glm::vec4 redColor(.8f, .2f, .3, 1.f);

			mBlueShader->bind();

			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(.2f * x, .2f * y, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(.1f, .1f, .1f));
					mBlueShader->setUniform("uColor", ((x + y) % 2 == 0 ? blueColor : redColor));
					zr::Renderer::Submit(mBlueShader, mSquareVA, transform);
				}
			}
			zr::Renderer::EndScene();
		}*/

		mLastDeltaTime = elapsedTime;
	}

	void SandboxLayer::onImGuiRender()
	{
		ImGui::Begin("Settings");
		{
			ImGui::ColorEdit3("Squares' color", &mSquareColor[0]);

			// Animate a simple progress bar
			//static float progress = 0.0f, progress_dir = 1.0f;
			/*progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
			if (progress >= +1.1f) {
				progress = +1.1f; progress_dir *= -1.0f;
			}
			if (progress <= -0.1f) {
				progress = -0.1f; progress_dir *= -1.0f;
			}*/

			// Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width, 
			// or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
			for (unsigned i = 0; i < mModels.size(); ++i) {
				ImGui::ProgressBar(mModels[i]->getLoadingProgress(), ImVec2(0.0f, 0.0f));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				ImGui::Text("Model loading progress %i", i);
				if (mModels[i]->isLoaded() && mModels[i]->hasAnimations()) {
					std::string startButtonId = "Start animation#" + std::to_string(i);
					std::string stopButtonId = "Stop animation#" + std::to_string(i);
					if (ImGui::Button(startButtonId.c_str())) {
						mModels[i]->setAnimation(0, true);
					} ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					if (ImGui::Button(stopButtonId.c_str())) {
						mModels[i]->stopAnimation();
					}
				}
			}
			ImGui::End();
		}
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
		mOrthographicCameraController->onEvent(e);
		mPerspectiveCameraController->onEvent(e);

		if (e.getType() == zr::EventType::MouseMove) {
			if (zr::Input::IsKeyPressed(zr::Keyboard::X)) {
				mModelScaleFactor -= zr::MouseMoveEvent::GetMovementOffset().y * .001f * mLastDeltaTime.asSeconds();
				if (mModelScaleFactor <= .001f) {
					mModelScaleFactor = .001f;
				}
			}
			else if (mIsMouseCaptured) {
				glm::vec2& movementOffset = zr::MouseMoveEvent::GetMovementOffset();
				e.setHandled();
				//std::cout << "Mouse movement offset: " << movementOffset.x << ", " << movementOffset.y << "\n";
				//mPerspectiveCamera->move({ movementOffset.x * mCameraSpeed * mLastDeltaTime.count(), movementOffset.y * mCameraSpeed * mLastDeltaTime.count(), 0.f });
				mFPSCamera->processMouseMovement(movementOffset * mCameraSpeed * mLastDeltaTime.asSeconds());
			}
			else if (zr::Input::IsMouseButtonPressed(zr::MouseButton::ButtonLeft)) {
				glm::vec2& movementOffset = zr::MouseMoveEvent::GetMovementOffset();
				e.setHandled();
				//std::cout << "Mouse movement offset: " << movementOffset.x << ", " << movementOffset.y << "\n";
				//mPerspectiveCamera->move({ movementOffset.x * mCameraSpeed * mLastDeltaTime.count(), movementOffset.y * mCameraSpeed * mLastDeltaTime.count(), 0.f });
				mFPSCamera->processMouseMovement(movementOffset * mCameraSpeed * mLastDeltaTime.asSeconds());
			}

			if (zr::Input::IsMouseButtonPressed(zr::MouseButton::ButtonRight)) {
				glm::vec2& movementOffset = zr::MouseMoveEvent::GetMovementOffset();
				e.setHandled();
			}
		}

		if (e.getType() == zr::EventType::KeyPressed) {
			if (dynamic_cast<zr::KeyPressedEvent&>(e).getKeyCode() == static_cast<int>(zr::Keyboard::Escape)) {
				e.setHandled();
				zr::Application::CloseWindow();
			}

			if (dynamic_cast<zr::KeyPressedEvent&>(e).getKeyCode() == static_cast<int>(zr::Keyboard::C)) {
				e.setHandled();
				mIsMouseCaptured = !mIsMouseCaptured;
				zr::Application::CaptureMouseCursor(mIsMouseCaptured);
				mFPSCamera->invertMouseMovement(!mIsMouseCaptured);
			}
		}
	}
}
