#include <pch.h>

#include "SandboxLayer.h"

#include "Zero/Zero/Input.h"
#include "Zero/Zero/Events.h"
#include "Zero/Zero/InputCodes.h"

namespace lp
{
	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer")
	{
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
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

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
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

	void SandboxLayer::onUpdate()
	{
		zr::RenderCommand::SetClearColor(1, 0, 1, 1);
		zr::RenderCommand::Clear();

		zr::Renderer::BeginScene();
		{
			mBlueShader->bind();
			zr::Renderer::Submit(mSquareVA);

			mShader->bind();
			zr::Renderer::Submit(mVertexArray);
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
