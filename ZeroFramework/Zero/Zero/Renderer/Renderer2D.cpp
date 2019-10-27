#include <zr_pch.h>

#include "Renderer2D.h"
#include "RenderCommand.h"

namespace zr
{
	void Renderer2D::Init()
	{
		sData = new Renderer2DStorage;
	
		sData->QuadVertexArray = VertexArray::Create(true);

		float squareVertices[5 * 4]{
			-.5f, -.5f, 0.0f,
			 .5f, -.5f, 0.0f,
			 .5f,  .5f, 0.0f,
			-.5f,  .5f, 0.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices), DrawMode::Static);
		squareVB->setLayout({
			{ShaderDataType::Float3, "aPosition"}
		});
		sData->QuadVertexArray->addVertexBuffer(squareVB);

		unsigned squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, 6, DrawMode::Static);
		sData->QuadVertexArray->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			void main()
			{
				gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			// layout (location = 0) out vec4 color;
			out vec4 color;
			uniform vec4 uColor;

			void main()
			{
				color = uColor;
			}
		)";

		sData->FlatColorShader = Shader::Create();
		sData->FlatColorShader->loadFromStrings(vertexSrc, fragmentSrc);
	}

	void Renderer2D::Shutdown()
	{
		delete sData;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		sData->FlatColorShader->bind();
		sData->FlatColorShader->setUniform("uViewProjection", camera->getViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.f}, size, angle, color);
	}
		
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		sData->FlatColorShader->bind();
		sData->FlatColorShader->setUniform("uColor", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(angle), { .0f, .0f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		sData->FlatColorShader->setUniform("uTransform", transform);

		sData->QuadVertexArray->bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}
}
