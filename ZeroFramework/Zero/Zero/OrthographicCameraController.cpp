#include "OrthographicCameraController.h"
#include "OrthographicCameraController.h"
#include <zr_pch.h>

#include "OrthographicCameraController.h"

#include <Zero/Core/Input.h>
#include <Zero/Core/EventDispatcher.h>
#include "Zero/Core/Profiller.h"

#include "Core/Log.h"

namespace zr
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		CameraController(),
		mAspectRatio(aspectRatio),
		mZoomLevel(1.f),
		mRotation(rotation),
		mCamera(nullptr),
		mCameraPosition(0.f, 0.f, 0.f),
		mCameraRotation(0.f),
		mCameraRotationSpeed(45.f),
		mCameraTranslationSpeed(5.f),
		mZoomRate(.25f),
		mMaxZoomLevel(.25f),
		mMinZoomLevel(std::numeric_limits<float>::max())
	{
		mCamera = CreateRef<OrthographicCamera>(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
	}

	OrthographicCameraController::OrthographicCameraController(float width, float height, bool rotation) :
		CameraController(),
		mAspectRatio(width / height),
		mZoomLevel(1.f),
		mRotation(rotation),
		mCamera(nullptr),
		mCameraPosition(0.f, 0.f, 0.f),
		mCameraRotation(0.f),
		mCameraRotationSpeed(45.f),
		mCameraTranslationSpeed(5.f),
		mZoomRate(.25f),
		mMaxZoomLevel(.25f),
		mMinZoomLevel(std::numeric_limits<float>::max())
	{
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
	}

	void OrthographicCameraController::onUpdate(const Time& elapsedTime)
	{
		ZR_PROFILER_FUNCTION();

		if (mInputMapper->isInputPressed(zr::CameraController::Actions::MoveCameraUp)) {
			mCameraPosition += glm::vec3(0.f, mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f);
		}

		if (mInputMapper->isInputPressed(zr::CameraController::Actions::MoveCameraDown)) {
			mCameraPosition += glm::vec3(0.f, -mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f);
		}

		if (mInputMapper->isInputPressed(zr::CameraController::Actions::MoveCameraLeft)) {
			mCameraPosition += glm::vec3(mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f, 0.f);
		}

		if (mInputMapper->isInputPressed(zr::CameraController::Actions::MoveCameraRight)) {
			mCameraPosition += glm::vec3(-mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f, 0.f);
		}

		if (mRotation) {
			if (mInputMapper->isInputPressed(zr::CameraController::Actions::RotateCameraLeft)) {
				mCameraRotation += mCameraRotationSpeed * elapsedTime.asSeconds();
			}

			if (mInputMapper->isInputPressed(zr::CameraController::Actions::RotateCameraRight)) {
				mCameraRotation -= mCameraRotationSpeed * elapsedTime.asSeconds();
			}

			mCamera->setRotationAngle(mCameraRotation);
		}

		mCamera->setPosition(mCameraPosition);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		ZR_PROFILER_FUNCTION();

		EventDispatcher eventDispatcher(e);
		eventDispatcher.dispatch<MouseScrollEvent>(std::bind(&OrthographicCameraController::onMouseScrolled, this, std::placeholders::_1));
		eventDispatcher.dispatch<RenderWindowResizeEvent>(std::bind(&OrthographicCameraController::onRenderWindowResized, this, std::placeholders::_1));
	}

	const Ref<Camera>& OrthographicCameraController::getCamera() const
	{
		return mCamera;
	}

	const Ref<InputMapper16>& OrthographicCameraController::getInputMapper() const
	{
		if (!mInputMapper) {
			mInputMapper = CreateRef<InputMapper16>();
		}

		return mInputMapper;
	}

	void OrthographicCameraController::enableRotation(bool enabled)
	{
		mRotation = enabled;
	}

	void OrthographicCameraController::setZoomLevel(float zoomLevel)
	{
		mZoomLevel = zoomLevel;
	}

	float OrthographicCameraController::getZoomLevel() const
	{
		return mZoomLevel;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrollEvent& e)
	{
		ZR_PROFILER_FUNCTION();

		float zoomLevel = mZoomLevel - e.getYOffset() * mZoomRate;
		zoomLevel = zr::clamp(mMaxZoomLevel, mMinZoomLevel, zoomLevel);
		readjustCameraTranslationSpeed(zoomLevel);
		updateProjectionMatrix();
		return false;
	}

	bool OrthographicCameraController::onRenderWindowResized(RenderWindowResizeEvent& e)
	{
		ZR_PROFILER_FUNCTION();

		mAspectRatio = (float)e.getWidth() / (float)e.getHeight();
		updateProjectionMatrix();
		return true;
	}

	bool OrthographicCameraController::onRenderWindowFramebufferResized(RenderWindowFramebufferResizeEvent& e)
	{
		mAspectRatio = (float)e.getWidth() / (float)e.getHeight();
		updateProjectionMatrix();
		return true;
	}

	void OrthographicCameraController::readjustCameraTranslationSpeed(float zoomLevel)
	{
		float diff = zoomLevel / mZoomLevel;
		mCameraTranslationSpeed *= diff;
		mZoomLevel = zoomLevel;
	}

	void OrthographicCameraController::updateProjectionMatrix()
	{
		dynamic_cast<OrthographicCamera&>(*mCamera).setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
	}
}
