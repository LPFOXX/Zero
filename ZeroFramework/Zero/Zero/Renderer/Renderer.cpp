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

	void Renderer::BeginScene(const std::shared_ptr<Camera>& camera, const std::shared_ptr<zr::Framebuffer>& framebuffer)
	{
		Renderer::sSceneData->ViewProjectionMatrix = camera->getViewProjectionMatrix();
		Renderer::sSceneData->Framebuffer = framebuffer;

		if (Renderer::sSceneData->Framebuffer != nullptr) {
			Renderer::sSceneData->Framebuffer->bind();
		}
		else {
			Framebuffer::BindDefault();
		}
	}

	void Renderer::EndScene()
	{
		if (Renderer::sSceneData->Framebuffer != nullptr) {
			zr::Framebuffer::BindDefault();
			Renderer::sSceneData->Framebuffer->draw();
		}
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->setUniform("u_ViewProjection", sSceneData->ViewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	
	void Renderer::Submit(const std::shared_ptr<CubeMap>& cubeMap)
	{
		cubeMap->render(Renderer::sSceneData->ViewProjectionMatrix);
	}
}
