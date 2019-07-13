#include <pch.h>

#include "SandboxLayer.h"

#include "Zero/Zero/Input.h"
#include "Zero/Zero/Events.h"
#include "Zero/Zero/InputCodes.h"

namespace lp
{
	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer"),
		mCamera(nullptr)
	{
		mCamera = std::shared_ptr<zr::Camera>(new zr::OrthographicCamera(-3.2f, 3.2f, -1.5f, 1.5f));


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

		mShader.reset(zr::Shader::create());
		if (!mShader->loadFromStrings(vertexSrc, fragmentSrc)) {
			std::cout << "Error creating Shader object!\n";
		}

		mBlueShader.reset(zr::Shader::create());
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
		zr::RenderCommand::SetClearColor(1, 0, 1, 1);
		zr::RenderCommand::Clear();

		if (zr::Input::isKeyPressed(zr::Keyboard::Up) || zr::Input::isKeyPressed(zr::Keyboard::W)) {
			mCamera->move({ 0.f, 1.f * elapsedTime.asSeconds(), 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Down) || zr::Input::isKeyPressed(zr::Keyboard::S)) {
			mCamera->move({ 0.f, -1.f * elapsedTime.asSeconds(), 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Left) || zr::Input::isKeyPressed(zr::Keyboard::A)) {
			mCamera->move({ -1.f * elapsedTime.asSeconds(), 0.f, 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Right) || zr::Input::isKeyPressed(zr::Keyboard::D)) {
			mCamera->move({ 1.f * elapsedTime.asSeconds(), 0.f, 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Q)) {
			mCamera->rotate(mCameraRotationSpeed * elapsedTime.asSeconds());
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::E)) {
			mCamera->rotate(-mCameraRotationSpeed * elapsedTime.asSeconds());
		}

		zr::Renderer::BeginScene(mCamera);
		{
			zr::Renderer::Submit(mBlueShader, mSquareVA);
			zr::Renderer::Submit(mShader, mVertexArray);
			zr::Renderer::EndScene();
		}
	}

	void SandboxLayer::OnImGuiRender()
	{
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
	}
}
