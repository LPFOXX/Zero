#pragma once

#include <Zero/Zero.h>

namespace lp
{
	class SandboxLayer : public zr::Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		std::shared_ptr<zr::VertexArray> mVertexArray;
		std::shared_ptr<zr::Shader> mShader;

		std::shared_ptr<zr::Shader> mBlueShader;
		std::shared_ptr<zr::VertexArray> mSquareVA;
	};
}
