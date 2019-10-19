#include <pch.h>

#include "SandboxLayer.h"
#include <Zero/Zero.h>

#include "Zero/vendor/glm/include/glm/gtc/matrix_transform.hpp"

namespace lp
{
	struct ModelLoader
	{
		ModelLoader()
		{
		}

		~ModelLoader()
		{
		}

		void operator()()
		{
			ZR_INFO("Model loading thread execution started.");
		}
	};

	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer"),
		mOrthographicCameraController(nullptr),
		mPerspectiveCamera(nullptr),
		mFramebuffer(nullptr),
		mFPSCamera(nullptr),
		mModelLoadingThread(&SandboxLayer::loadModel, this),
		mModelLoadingMutex()
	{
		/*zr::ModelLoader::Get()->setProgressHandler(std::make_shared<zr::ModelProgressHandler>());
		zr::Ref<zr::ModelLogger> modelLogger = std::make_shared<zr::ModelLogger>();
		modelLogger->setLogSeverity(Assimp::Logger::LogSeverity::VERBOSE);
		zr::ModelLoader::Get()->setLogger(modelLogger);*/

		//mModelLoadingThread.launch();
		mOrthographicCameraController = std::shared_ptr<zr::OrthographicCameraController>(new zr::OrthographicCameraController(1280.f / 600.f, true));
		mPerspectiveCamera = std::shared_ptr<zr::Camera>(new zr::PerspectiveCamera(45.f, 1280, 600));
		mFPSCamera.reset(new zr::FPSCamera({ 0.f, 0.f, 3.f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f }, 1280.f, 600.f));
		mFPSCamera->invertMouseMovement(!mIsMouseCaptured);

		/*mModel = std::make_shared<zr::Model3D>("resources/nanosuit/nanosuit.obj", zr::MeshData::Normals | zr::MeshData::TextureCoordinates, [&](float& progress) {
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

		zr::Framebuffer::FramebufferProperties fbp;
		fbp.Width = 1280;
		fbp.Height = 600;
		fbp.MSSALevel = 8;

		mFramebuffer.reset(zr::Framebuffer::Create(fbp));

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
		if (!mShader->loadFromStrings(vertexSrc, fragmentSrc)) {
			ZR_CORE_ERROR("Error creating Shader object!");
		}

		mBlueShader = zr::Shader::Create();
		if (!mBlueShader->loadFromStrings(blueShaderVertexSrc, blueShaderFragmentSrc)) {
			ZR_CORE_ERROR("Error creating blue Shader object!");
		}
	}

	SandboxLayer::~SandboxLayer()
	{
		mModelLoadingThread.wait();
	}

	void SandboxLayer::onAttach()
	{
	}

	void SandboxLayer::onDetach()
	{
	}

	void SandboxLayer::onUpdate(const zr::Time& elapsedTime)
	{
		mOrthographicCameraController->onUpdate(elapsedTime);

		if (zr::Input::isKeyPressed(zr::Keyboard::Up)) {
			mPerspectiveCamera->move({ 0.f, mCameraSpeed * elapsedTime.asSeconds(), 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Forward, elapsedTime);
		}
		
		if (zr::Input::isKeyPressed(zr::Keyboard::Down)) {
			mPerspectiveCamera->move({ 0.f, -mCameraSpeed * elapsedTime.asSeconds(), 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Backward, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Left)) {
			mPerspectiveCamera->move({ -mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Left, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Right)) {
			mPerspectiveCamera->move({ mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Right, elapsedTime);
		}

		const zr::Time& time = zr::Application::GetTime();
		//mFPSCamera->setPosition({ 1.f * cos(time.asSeconds()), .0f, 1.f * sin(time.asSeconds()) });

		zr::RenderCommand::EnableDepthTest(true);

		glm::mat4 model(1.f);
		model = glm::scale(model, glm::vec3(mModelScaleFactor, mModelScaleFactor, mModelScaleFactor));
		/*mModel->setTransformationMatrix(model);
		mModel->setCameraPosition(mFPSCamera->getPosition());*/

		//zr::Renderer::BeginScene(mFPSCamera);
		//{
		//	zr::RenderCommand::SetClearColor(.2f, .2f, .2f, 1.f);
		//	zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

		//mModel->render(mFPSCamera->getViewProjectionMatrix());

		//	//zr::RenderCommand::EnableFaceCulling(true, zr::RendererAPI::CullFace::Front);
		//	//zr::RenderCommand::EnableFaceCulling(false);
		//	/*zr::Renderer::Submit(mBlueShader, mSquareVA);
		//	zr::Renderer::Submit(mShader, mVertexArray);*/
		//	zr::Renderer::Submit(mCubeMap, true);
		//	zr::Renderer::EndScene();
		//}

		//zr::Renderer::BeginScene(std::make_shared<zr::OrthographicCamera>(mOrthographicCameraController->getCamera()));
		zr::Renderer::BeginScene(mOrthographicCameraController->getCamera());
		{
			zr::RenderCommand::SetClearColor(1.f, 0.f, 1.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			//mModel->render(mOrthographicCameraController->getCamera().getViewProjectionMatrix());

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

			/*zr::Renderer::Submit(mShader, mVertexArray);
			zr::Renderer::Submit(mCubeMap);*/
			zr::Renderer::EndScene();
		}

		mLastDeltaTime = zr::Time::Seconds(elapsedTime.asSeconds());
	}

	void SandboxLayer::OnImGuiRender()
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
			ImGui::ProgressBar(mProgress, ImVec2(0.0f, 0.0f));
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::Text("Progress Bar");
			ImGui::End();
		}

		static zr::ImGuiConsole imguiConsole;
		static bool isOpen = true;
		imguiConsole.Draw("Example: Console", &isOpen);
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
		mOrthographicCameraController->onEvent(e);
		if (e.getType() == zr::EventType::WindowResize) {
			//auto& resizeEvent = (zr::WindowResizeEvent&) e;
			//mOrthographicCameraController->setZoomLevel(resizeEvent.fe)
		}



		if (e.getType() == zr::EventType::MouseMove) {
			if (zr::Input::isKeyPressed(zr::Keyboard::X)) {
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
			else if (zr::Input::isMouseButtonPressed(zr::MouseButton::Button0)) {
				glm::vec2& movementOffset = zr::MouseMoveEvent::GetMovementOffset();
				e.setHandled();
				//std::cout << "Mouse movement offset: " << movementOffset.x << ", " << movementOffset.y << "\n";
				//mPerspectiveCamera->move({ movementOffset.x * mCameraSpeed * mLastDeltaTime.count(), movementOffset.y * mCameraSpeed * mLastDeltaTime.count(), 0.f });
				mFPSCamera->processMouseMovement(movementOffset * mCameraSpeed * mLastDeltaTime.asSeconds());
			}

			if (zr::Input::isMouseButtonPressed(zr::MouseButton::Button1)) {
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

	void SandboxLayer::loadModel()
	{
		ZR_INFO("Model loading started.");
		/*zr::ModelLoader::Get()->loadFromFile("resources/nanosuit/nanosuit.obj", zr::MeshData::Normals | zr::MeshData::TextureCoordinates, [&](float& progress) {
			mProgress = progress;
		});*/
		//zr::Ref<zr::Model> temp = std::make_shared<zr::Model3D>("resources/nanosuit/nanosuit.obj");
		//temp->loadFromFile("resources/iron_man/IronMan.obj", zr::Model3D::LoadComponents::Normals | zr::Model3D::LoadComponents::TextureCoordinates);
		//temp->sub
		//mModel.reset(new zr::Model("resources/iron_man/IronMan.obj", zr::Model::LoadComponents::Normals | zr::Model::LoadComponents::TextureCoordinates));
		//mModel.reset(new zr::Model("resources/iron_man_fixed/iron_man_fixed.obj", zr::Model::LoadComponents::Normals | zr::Model::LoadComponents::TextureCoordinates));
		//mModel.reset(new zr::Model("resources/ghost/disk_g.obj"));

		//progress = temp->getLoadingProgress();

		{
			sf::Lock lock(mModelLoadingMutex);
			//mModel = temp;
		}
		onLoadModelFinished();
	}

	void SandboxLayer::onLoadModelFinished()
	{
		ZR_INFO("Model loading finished.");
	}
}
