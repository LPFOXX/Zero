#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

#include "Renderer/Camera.h"
#include "Core/Clock.h"

#include "Core/Events/WindowResizeEvent.h"
#include "Core/Events/MouseScrollEvent.h"

#include "Core/Core.h"

namespace zr
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		OrthographicCameraController(float width, float height, bool rotation = false);
		virtual ~OrthographicCameraController();

		void onUpdate(const Time& elapsedTime);
		void onEvent(Event& e);

		const Ref<OrthographicCamera>& getCamera() const;

		void setZoomLevel(float zoomLevel);
		float getZoomLevel() const;

	private:
		bool onMouseScrolled(MouseScrollEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
		void readjustCameraTranslationSpeed(float zoomLevel);
		void updateProjectionMatrix();

	private:
		float mAspectRatio;
		float mZoomLevel;
		bool mRotation;
		Ref<OrthographicCamera> mCamera;

		glm::vec3 mCameraPosition;
		float mCameraRotation;
		float mCameraRotationSpeed;
		float mCameraTranslationSpeed;

		float mZoomRate;
		float mMaxZoomLevel;
		float mMinZoomLevel;
	};
}
