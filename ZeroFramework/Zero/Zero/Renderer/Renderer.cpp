#include <zr_pch.h>

#include "Renderer.h"
#include "Renderer2D.h"
#include "Texture.h"

namespace zr
{
	Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<Renderer::SceneData>();

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
	
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene(const Ref<Camera>& camera, const Ref<zr::Framebuffer>& framebuffer)
	{
		Renderer::sSceneData->Camera = &(*camera);
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
			Framebuffer::BindDefault();
			Texture2D::ActivateTextureSlot(0, 0);
			Renderer::sSceneData->Framebuffer->draw();
		}
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		shader->setUniform("uViewProjection", sSceneData->Camera->getViewProjectionMatrix());
		shader->setUniform("uTransform", transform);

		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<CubeMap>& cubeMap, bool ignoreTranslations)
	{
		if (ignoreTranslations) {
			const glm::mat4& projection = Renderer::sSceneData->Camera->getProjectionMatrix();
			glm::mat4 view = glm::mat3(Renderer::sSceneData->Camera->getViewMatrix());
			cubeMap->render(projection * view);
			return;
		}
		
		cubeMap->render(Renderer::sSceneData->Camera->getViewProjectionMatrix());
	}

	void Renderer::Submit(const Ref<Text>& text)
	{
		text->render(Renderer::sSceneData->Camera->getViewProjectionMatrix());
	}

	void Renderer::Submit(const Ref<Sprite>& sprite)
	{
		sprite->render(Renderer::sSceneData->Camera->getViewProjectionMatrix());
	}
}
