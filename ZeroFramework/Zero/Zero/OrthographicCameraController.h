#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

#include "Renderer/Camera.h"
#include "Core/Clock.h"

#include "Core/Events/WindowResizeEvent.h"
#include "Core/Events/MouseScrollEvent.h"

#include "CameraController.hpp"

#include "Core/Core.h"

namespace zr
{
	class OrthographicCameraController : public CameraController
	{
	public:
		OrthographicCameraController(float aspectRatio = (16.f / 9.f), bool rotation = false);
		OrthographicCameraController(float width, float height, bool rotation = false);
		virtual ~OrthographicCameraController();

		virtual void onUpdate(const Time& elapsedTime) override;
		virtual void onEvent(Event& e) override;

		virtual const Ref<Camera>& getCamera() const override;
		virtual const Ref<InputMapper16>& getInputMapper() const override;
		virtual void enableRotation(bool enabled) override;

		void setZoomLevel(float zoomLevel);
		float getZoomLevel() const;

	private:
		bool onRenderWindowResized(RenderWindowResizeEvent& e);
		bool onRenderWindowFramebufferResized(RenderWindowFramebufferResizeEvent& e);
		void readjustCameraTranslationSpeed(float zoomLevel);
		void updateProjectionMatrix();

	private:
		float mAspectRatio;
		float mZoomLevel;
		bool mRotation;
		Ref<Camera> mCamera;

		glm::vec3 mCameraPosition;
		float mCameraRotation;
		float mCameraRotationSpeed;
		float mCameraTranslationSpeed;

		float mZoomRate;
		float mMaxZoomLevel;
		float mMinZoomLevel;

		mutable Ref<InputMapper16> mInputMapper;
	};
}
