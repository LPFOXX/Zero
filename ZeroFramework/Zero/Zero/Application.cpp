#include <zr_pch.h>

#include "Events.h"
#include "Application.h"
#include "EventDispatcher.h"
#include "Input.h"
#include "Zero/Renderer/Renderer.h"

namespace zr
{
	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		mWindow.reset(Window::Create());
		Application::sInstance = this;
		mWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		mImGuiLayer = new ImGuiLayer();
		pushOverlay(mImGuiLayer);

		float vertices[3 * 7] {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		unsigned indices[3]{
			0, 1, 2
		};

		mVertexArray.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices), DrawMode::Static));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->setLayout(layout);
		mVertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3U, DrawMode::Static));
		mVertexArray->setIndexBuffer(indexBuffer);


		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		mSquareVA.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices), DrawMode::Static));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		mSquareVA->addVertexBuffer(squareVB);

		unsigned squareIndices[6] { 
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6U, DrawMode::Static));
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

		mShader.reset(Shader::create());
		if (!mShader->loadFromStrings(vertexSrc, fragmentSrc)) {
			std::cout << "Error creating Shader object!\n";
		}

		mBlueShader.reset(Shader::create());
		if (!mBlueShader->loadFromStrings(blueShaderVertexSrc, blueShaderFragmentSrc)) {
			std::cout << "Error creating blue Shader object!\n";
		}
	}

	Application::~Application()
	{

	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>([](WindowResizeEvent& e) -> bool {
			RenderCommand::SetViewportSize(e.getWidth(), e.getHeight());
			return true;
		});

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it) {
			(*it)->onEvent(e);
			if (e.isHandled()) {
				break;
			}
		}

		std::cout << e.toString() << '\n';
	}

	void Application::pushLayer(Layer* layer)
	{
		mLayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		mLayerStack.pushOverlay(layer);
	}

	void Application::run()
	{
		while (mRunning) {

			RenderCommand::SetClearColor(1, 0, 1, 1);
			RenderCommand::Clear();

			Renderer::BeginScene();
			{
				mBlueShader->bind();
				Renderer::Submit(mSquareVA);

				mShader->bind();
				Renderer::Submit(mVertexArray);
				Renderer::EndScene();
			}

			for (Layer* l : mLayerStack) {
				l->onUpdate();
			}

			mImGuiLayer->begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->end();

			mWindow->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}
}
