#pragma once

#include "../Core/EventDispatcher.h"
#include "Framebuffer.h"
#include "../Core/Core.h"
#include "../CameraController.hpp"
#include "../Core/Input.h"
#include "../Renderer/RenderCommand.h"

namespace zr
{
	class Viewer
	{
	public:
		Viewer() :
			mCameraController(nullptr),
			mFramebuffer(nullptr),
			mRenderWindowSize(0.f, 0.f),
			mIsEditable(false)
		{

		}

		Viewer(unsigned width, unsigned height, unsigned mssaLevel) :
			mCameraController(nullptr),
			mFramebuffer(nullptr)
		{
			mFramebuffer = Framebuffer::Create({ width, height, mssaLevel });
		}

		void onUpdate(const Time& elapsedTime)
		{
			mCameraController->onUpdate(elapsedTime);
		}

		void onEvent(Event& e)
		{
			EventDispatcher d(e);
			d.dispatch<RenderWindowResizeEvent>(std::bind(&Viewer::onRenderWindowResize, this, std::placeholders::_1));
			//d.dispatch<RenderWindowFramebufferResizeEvent>(std::bind(&Viewer::onRenderWindowFramebufferResized, this, std::placeholders::_1));
			mCameraController->onEvent(e);
		}

		void bind()
		{
			auto& props = mFramebuffer->getProperties();
			mFramebuffer->bind();
			RenderCommand::SetViewportSize(props.Width, props.Height);
		}

		void bind(unsigned bufferBits, float r, float g, float b, float a)
		{
			bind();

			zr::RenderCommand::Clear(bufferBits);
			zr::RenderCommand::SetClearColor(r, g, b, a);
		}

		void setCameraController(CameraController* cameraController)
		{
			mCameraController.reset(cameraController);
		}

		const Ref<CameraController>& getCameraController() const
		{
			return mCameraController;
		}

		void setCameraController(const Ref<CameraController>& cameraController)
		{
			mCameraController = cameraController;
		}

		const Ref<Camera>& getCamera() const
		{
			return mCameraController->getCamera();
		}

		void setFramebuffer(const Ref<Framebuffer>& framebuffer)
		{
			mFramebuffer = framebuffer;
		}

		const Ref<Framebuffer>& getFramebuffer() const
		{
			return mFramebuffer;
		}

		bool isEditable() const
		{
			return mIsEditable;
		}

		void setEditable(bool isEditable)
		{
			mIsEditable = isEditable;
		}

		const glm::vec2& getRenderWindowSize()
		{
			return mRenderWindowSize;
		}

		const Ref<InputMapper16>& getInput() const
		{
			return mCameraController->getInputMapper();
		}

	private:
		bool onRenderWindowResize(RenderWindowResizeEvent& e)
		{
			mRenderWindowSize.x = (float)e.getWidth();
			mRenderWindowSize.y = (float)e.getHeight();
			return true;
		}

		//bool onRenderWindowFramebufferResized(RenderWindowFramebufferResizeEvent& e)
		//{
		//	//RenderCommand::SetViewportSize(e.getWidth(), e.getHeight());
		//	return true;
		//}

	public:
		bool mIsEditable;

	private:
		Ref<CameraController> mCameraController;
		Ref<Framebuffer> mFramebuffer;
		glm::vec2 mRenderWindowSize;
	};
}
