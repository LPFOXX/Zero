#include <pch.h>

#include "SandboxLayer.h"
#include <Zero/Zero.h>

#include "Zero/vendor/glm/include/glm/gtc/matrix_transform.hpp"

namespace lp
{
	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer"),
		mOrthographicCamera(nullptr),
		mPerspectiveCamera(nullptr),
		mFramebuffer(nullptr),
		mFPSCamera(nullptr)
	{
		mOrthographicCamera = std::shared_ptr<zr::Camera>(new zr::OrthographicCamera(0.f, 3.2f, 0.f, 1.5f));
		mPerspectiveCamera = std::shared_ptr<zr::Camera>(new zr::PerspectiveCamera(45.f, 1280, 600));
		mFPSCamera.reset(new zr::FPSCamera({ 0.f, 0.f, 3.f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f }, 1280.f, 600.f));

		mModel.reset(new zr::Model("resources/nanosuit/nanosuit.obj"));

		mCubeMap.reset(zr::CubeMap::Create());
		bool success = mCubeMap->loadFromFiles({
			"resources/skybox/right.jpg",
			"resources/skybox/left.jpg",
			"resources/skybox/top.jpg",
			"resources/skybox/bottom.jpg",
			"resources/skybox/back.jpg",
			"resources/skybox/front.jpg"
			});

		if (!success) {
			std::cout << "CubeMap loading error\n";
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

		mVertexArray.reset(zr::VertexArray::Create());

		std::shared_ptr<zr::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(zr::VertexBuffer::Create(vertices, sizeof(vertices), zr::DrawMode::Static));
		zr::BufferLayout layout = {
			{ zr::ShaderDataType::Float3, "a_Position" },
			{ zr::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->setLayout(layout);
		mVertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<zr::IndexBuffer> indexBuffer;
		indexBuffer.reset(zr::IndexBuffer::Create(indices, 3U, zr::DrawMode::Static));
		mVertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		mSquareVA.reset(zr::VertexArray::Create());

		std::shared_ptr<zr::VertexBuffer> squareVB;
		squareVB.reset(zr::VertexBuffer::Create(squareVertices, sizeof(squareVertices), zr::DrawMode::Static));
		squareVB->setLayout({
			{ zr::ShaderDataType::Float3, "a_Position" }
			});
		mSquareVA->addVertexBuffer(squareVB);

		unsigned squareIndices[6]{
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<zr::IndexBuffer> squareIB;
		squareIB.reset(zr::IndexBuffer::Create(squareIndices, 6U, zr::DrawMode::Static));
		mSquareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		mShader.reset(zr::Shader::Create());
		if (!mShader->loadFromStrings(vertexSrc, fragmentSrc)) {
			std::cout << "Error creating Shader object!\n";
		}

		mBlueShader.reset(zr::Shader::Create());
		if (!mBlueShader->loadFromStrings(blueShaderVertexSrc, blueShaderFragmentSrc)) {
			std::cout << "Error creating blue Shader object!\n";
		}
	}

	SandboxLayer::~SandboxLayer()
	{

	}

	void SandboxLayer::onAttach()
	{
	}

	void SandboxLayer::onDetach()
	{
	}

	void SandboxLayer::onUpdate(const zr::Time& elapsedTime)
	{
		if (zr::Input::isKeyPressed(zr::Keyboard::Up)) {
			mOrthographicCamera->move({ 0.f, mCameraSpeed * elapsedTime.asSeconds(), 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::W)) {
			mPerspectiveCamera->move({ 0.f, mCameraSpeed * elapsedTime.asSeconds(), 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Forward, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Down)) {
			mOrthographicCamera->move({ 0.f, -mCameraSpeed * elapsedTime.asSeconds(), 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::S)) {
			mPerspectiveCamera->move({ 0.f, -mCameraSpeed * elapsedTime.asSeconds(), 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Backward, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Left)) {
			mOrthographicCamera->move({ -mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::A)) {
			mPerspectiveCamera->move({ -mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Left, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Right)) {
			mOrthographicCamera->move({ mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::D)) {
			mPerspectiveCamera->move({ mCameraSpeed * elapsedTime.asSeconds(), 0.f, 0.f });
			mFPSCamera->move(zr::FPSCamera::MovementDirection::Right, elapsedTime);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Q)) {
			mOrthographicCamera->rotate(mCameraRotationSpeed * elapsedTime.asSeconds());
			mFPSCamera->rotate(mCameraRotationSpeed * elapsedTime.asSeconds());
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::E)) {
			mOrthographicCamera->rotate(-mCameraRotationSpeed * elapsedTime.asSeconds());
			mFPSCamera->rotate(-mCameraRotationSpeed * elapsedTime.asSeconds());
		}

		const zr::Time& time = zr::Application::GetTime();
		//mPerspectiveCamera->setPosition({ 1.f * cos(time.asSeconds()), .0f, 1.f * sin(time.asSeconds()) });

		zr::RenderCommand::EnableDepthTest(true);

		zr::Renderer::BeginScene(mFPSCamera);
		{
			zr::RenderCommand::SetClearColor(.2f, .2f, .2f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			glm::mat4 model(1.f);
			model = glm::translate(model, glm::vec3(1.f, 0.f, 0.f));
			mModel->setTransformationMatrix(model);
			mModel->render(mFPSCamera->getViewProjectionMatrix());

			//zr::RenderCommand::EnableFaceCulling(true, zr::RendererAPI::CullFace::Front);
			//zr::RenderCommand::EnableFaceCulling(false);
			/*zr::Renderer::Submit(mBlueShader, mSquareVA);
			zr::Renderer::Submit(mShader, mVertexArray);*/
			//zr::Renderer::Submit(mCubeMap, true);
			zr::Renderer::EndScene();
		}

		zr::Renderer::BeginScene(mOrthographicCamera, mFramebuffer);
		{
			zr::RenderCommand::SetClearColor(1.f, 0.f, 1.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			/*zr::Renderer::Submit(mBlueShader, mSquareVA);
			zr::Renderer::Submit(mShader, mVertexArray);*/
			zr::Renderer::Submit(mCubeMap);
			zr::Renderer::EndScene();
		}

		mLastDeltaTime = zr::Time::seconds(elapsedTime.asSeconds());
	}

	void SandboxLayer::OnImGuiRender()
	{
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
		if (e.getType() == zr::EventType::MouseMove) {
			if (mIsMouseCaptured) {
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
			}
		}
	}
}
