#pragma once

#include <Zero/Zero.h>

#include <SFML/System.hpp>

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
		void loadModel();
		void onLoadModelFinished();

	private:
		zr::Ref<zr::VertexArray> mVertexArray;
		zr::Ref<zr::Shader> mShader;

		zr::Ref<zr::Shader> mBlueShader;
		zr::Ref<zr::VertexArray> mSquareVA;

		zr::Ref<zr::OrthographicCameraController> mOrthographicCameraController;
		zr::Ref<zr::Camera> mPerspectiveCamera;
		zr::Ref<zr::FPSCamera> mFPSCamera;

		zr::Ref<zr::CubeMap> mCubeMap;

		//zr::Ref<zr::Model3D> mModel;
		sf::Thread mModelLoadingThread;
		sf::Mutex mModelLoadingMutex;
		float mModelScaleFactor = .01f;

		//float mCameraRotationSpeed = 45.f;	// 45 degrees per second
		float mCameraSpeed = 5.f;			// 5 units per second

		zr::Time mLastDeltaTime;
		float mProgress = 0.f;


		zr::Ref<zr::Framebuffer> mFramebuffer;

		glm::vec3 mSquareColor = {0.f, 0.f, 0.f};

		bool mIsMouseCaptured = false;
	};
}
