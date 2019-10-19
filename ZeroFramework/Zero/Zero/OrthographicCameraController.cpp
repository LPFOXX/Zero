#include <zr_pch.h>

#include "OrthographicCameraController.h"

#include <Zero/Core/Input.h>
#include <Zero/Core/EventDispatcher.h>

#include "Core/Log.h"

namespace zr
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		mAspectRatio(aspectRatio),
		mZoomLevel(1.f),
		mRotation(rotation),
		mCamera(nullptr),
		mCameraPosition(0.f, 0.f, 0.f),
		mCameraRotation(0.f),
		mCameraRotationSpeed(45.f),
		mCameraTranslationSpeed(5.f)
	{
		mCamera = std::shared_ptr<OrthographicCamera>(new OrthographicCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel));
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
	}

	void OrthographicCameraController::onUpdate(const Time& elapsedTime)
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

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.dispatch<MouseScrollEvent>(std::bind(&OrthographicCameraController::onMouseScrolled, this, std::placeholders::_1));
		eventDispatcher.dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::onWindowResized, this, std::placeholders::_1));
	}

	const Ref<OrthographicCamera>& OrthographicCameraController::getCamera() const
	{
		return mCamera;
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
		mZoomLevel -= e.getYOffset();
		mZoomLevel = std::max(mZoomLevel, .25f);
		mCamera->setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		mAspectRatio = (float)e.getWidth() / (float)e.getHeight();
		mCamera->setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}

	void OrthographicCameraController::readjustCameraTranslationSpeed()
	{
		mCameraTranslationSpeed = std::exp(1.f / mZoomLevel);
	}
}
