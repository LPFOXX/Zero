#include <zr_pch.h>

#include "Renderer.h"

namespace zr
{
	std::unique_ptr<Renderer::SceneData> Renderer::sSceneData(new Renderer::SceneData);

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		sSceneData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->setUniform("u_ViewProjection", sSceneData->ViewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
