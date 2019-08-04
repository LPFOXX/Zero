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
		mFPSCamera->invertMouseMovement(!mIsMouseCaptured);

		//mModel.reset(new zr::Model("resources/nanosuit/nanosuit.obj"));
		//mModel.reset(new zr::Model("resources/iron_man/IronMan.obj", zr::Model::LoadComponents::Normals | zr::Model::LoadComponents::TextureCoordinates));
		mModel.reset(new zr::Model("resources/iron_man_fixed/iron_man_fixed.obj", zr::Model::LoadComponents::Normals | zr::Model::LoadComponents::TextureCoordinates));
		//mModel.reset(new zr::Model("resources/ghost/disk_g.obj"));

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
			ZR_CORE_ERROR("CubeMap loading error.");
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
			{ zr::ShaderDataType::Float3, "aPosition" },
			{ zr::ShaderDataType::Float4, "aColor" }
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
			{ zr::ShaderDataType::Float3, "aPosition" }
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

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		mShader.reset(zr::Shader::Create());
		if (!mShader->loadFromStrings(vertexSrc, fragmentSrc)) {
			ZR_CORE_ERROR("Error creating Shader object!");
		}

		mBlueShader.reset(zr::Shader::Create());
		if (!mBlueShader->loadFromStrings(blueShaderVertexSrc, blueShaderFragmentSrc)) {
			ZR_CORE_ERROR("Error creating blue Shader object!");
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
		//mFPSCamera->setPosition({ 1.f * cos(time.asSeconds()), .0f, 1.f * sin(time.asSeconds()) });

		zr::RenderCommand::EnableDepthTest(true);

		glm::mat4 model(1.f);
		model = glm::scale(model, glm::vec3(mModelScaleFactor, mModelScaleFactor, mModelScaleFactor));
		mModel->setTransformationMatrix(model);
		mModel->setCameraPosition(mFPSCamera->getPosition());

		zr::Renderer::BeginScene(mFPSCamera);
		{
			zr::RenderCommand::SetClearColor(.2f, .2f, .2f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			mModel->render(mFPSCamera->getViewProjectionMatrix());

			//zr::RenderCommand::EnableFaceCulling(true, zr::RendererAPI::CullFace::Front);
			//zr::RenderCommand::EnableFaceCulling(false);
			/*zr::Renderer::Submit(mBlueShader, mSquareVA);
			zr::Renderer::Submit(mShader, mVertexArray);*/
			zr::Renderer::Submit(mCubeMap, true);
			zr::Renderer::EndScene();
		}

		//zr::Renderer::BeginScene(mOrthographicCamera, mFramebuffer);
		//{
		//	zr::RenderCommand::SetClearColor(1.f, 0.f, 1.f, 1.f);
		//	zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

		//	mModel->render(mOrthographicCamera->getViewProjectionMatrix());

		//	for (int x = 0; x < 10; x++) {
		//		for (int y = 0; y < 10; y++) {
		//			glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(.2f * x, .2f * y, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(.1f, .1f, .1f));
		//			zr::Renderer::Submit(mBlueShader, mSquareVA, transform);
		//		}
		//	}

		//	zr::Renderer::Submit(mShader, mVertexArray);
		//	//zr::Renderer::Submit(mCubeMap);
		//	zr::Renderer::EndScene();
		//}

		mLastDeltaTime = zr::Time::seconds(elapsedTime.asSeconds());
	}

	void SandboxLayer::OnImGuiRender()
	{
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
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
}
