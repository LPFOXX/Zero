#include <zr_pch.h>

#include "PerspectiveCameraController.h"

#include "Core/EventDispatcher.h"
#include "Core/Input.h"

namespace zr
{
	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, bool rotation) :
		mAspectRatio(aspectRatio),
		mFieldOfView(45.f),
		mRotation(rotation),
		mCamera(nullptr),
		mCameraPosition(0.f, 0.f, 0.f),
		mCameraRotation(0.f),
		mCameraRotationSpeed(45.f),
		mCameraTranslationSpeed(5.f)
	{
		mCamera = std::make_shared<PerspectiveCamera>(mAspectRatio, mFieldOfView);
	}

	PerspectiveCameraController::~PerspectiveCameraController()
	{
	}

	void PerspectiveCameraController::onUpdate(const Time& elapsedTime)
	{
		if (Input::isKeyPressed(Keyboard::W)) {
			mCameraPosition += glm::vec3(0.f, mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f);
		}

		if (Input::isKeyPressed(Keyboard::S)) {
			mCameraPosition += glm::vec3(0.f, -mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f);
		}

		if (Input::isKeyPressed(Keyboard::A)) {
			mCameraPosition += glm::vec3(-mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f, 0.f);
		}

		if (Input::isKeyPressed(Keyboard::D)) {
			mCameraPosition += glm::vec3(mCameraTranslationSpeed * elapsedTime.asSeconds(), 0.f, 0.f);
		}

		if (mRotation) {
			if (zr::Input::isKeyPressed(zr::Keyboard::Q)) {
				mCameraRotation += mCameraRotationSpeed * elapsedTime.asSeconds();
			}

			if (zr::Input::isKeyPressed(zr::Keyboard::E)) {
				mCameraRotation -= mCameraRotationSpeed * elapsedTime.asSeconds();
			}

			mCamera->setRotationAngle(mCameraRotation);
		}

		mCamera->setPosition(mCameraPosition);
		readjustCameraTranslationSpeed();
	}

	void PerspectiveCameraController::onEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.dispatch<MouseScrollEvent>(std::bind(&PerspectiveCameraController::onMouseScrolled, this, std::placeholders::_1));
		eventDispatcher.dispatch<WindowResizeEvent>(std::bind(&PerspectiveCameraController::onWindowResized, this, std::placeholders::_1));
	}

	const Ref<PerspectiveCamera>& PerspectiveCameraController::getCamera() const
	{
		return mCamera;
	}

	void PerspectiveCameraController::setFieldOfView(float angle)
	{
		mFieldOfView = angle;
		mFieldOfView = zr::clamp(0.f, 90.f, mFieldOfView);
		mCamera->setProjection(mAspectRatio, mFieldOfView);
	}

	float PerspectiveCameraController::getFieldOfView() const
	{
		return mFieldOfView;
	}

	bool PerspectiveCameraController::onMouseScrolled(MouseScrollEvent& e)
	{
		mFieldOfView -= e.getYOffset();
		mFieldOfView = zr::clamp(0.f, 90.f, mFieldOfView);
		mCamera->setProjection(mAspectRatio, mFieldOfView);
		return false;
	}

	bool PerspectiveCameraController::onWindowResized(WindowResizeEvent& e)
	{
		mAspectRatio = (float)e.getWidth() / (float)e.getHeight();
		mCamera->setProjection(mAspectRatio, mFieldOfView);
		return false;
	}

	void PerspectiveCameraController::readjustCameraTranslationSpeed()
	{
		mCameraTranslationSpeed = std::exp(1.f / mFieldOfView);
	}
}
