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
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		std::shared_ptr<zr::VertexArray> mVertexArray;
		std::shared_ptr<zr::Shader> mShader;

		std::shared_ptr<zr::Shader> mBlueShader;
		std::shared_ptr<zr::VertexArray> mSquareVA;

		std::shared_ptr<zr::Camera> mCamera;

		float mCameraRotationSpeed = 45.f;	// 45 degrees per second
		float mCameraSpeed = 5.f;			// 5 units per second

		std::shared_ptr<zr::Framebuffer> mFramebuffer;
	};
}
