#include <zr_pch.h>

#include "../../vendor/glm/include/glm/gtc/matrix_transform.hpp"

#include "FPSCamera.h"

namespace zr
{
	FPSCamera::FPSCamera(const glm::vec3& initialPosition, const glm::vec3& front, const glm::vec3& up, float width, float height, float fieldOfView) :
		Camera(),
		mPosition(initialPosition),
		mFront(front),
		mUp(up),
		mWorldUp(up),
		mRight(1.f, 0.f, 0.f),
		mYawAngle(0U),
		mPitchAngle(0U),
		mRollAngle(0U),
		mFieldOfView(fieldOfView)
	{
		float aspectRatio = 1.f;
		if (height != 0) {
			aspectRatio = width / height;
		}
		mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), aspectRatio, 0.1f, 10.f);
		mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	FPSCamera::~FPSCamera()
	{
	}

	void FPSCamera::processKeyboard(Keyboard key)
	{
	}

	void FPSCamera::processMouseMovement(const glm::vec2& mouseOffset)
	{
		mYawAngle += -mouseOffset.x;
		mPitchAngle += mouseOffset.y;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (mPitchAngle > 89.0f)
			mPitchAngle = 89.0f;
		if (mPitchAngle < -89.0f)
			mPitchAngle = -89.0f;

		// Update Front, Right and Up Vectors using the updated Eular angles
		recomputeMatrices();
	}

	void FPSCamera::move(MovementDirection direction, const Time& deltaTime)
	{
		if (direction == MovementDirection::Forward) {
			mPosition += mFront * deltaTime.asSeconds();
		}

		if (direction == MovementDirection::Backward) {
			mPosition -= mFront * deltaTime.asSeconds();
		}

		if (direction == MovementDirection::Right) {
			mPosition += mRight * deltaTime.asSeconds();
		}

		if (direction == MovementDirection::Left) {
			mPosition -= mRight * deltaTime.asSeconds();
		}

		recomputeMatrices();
	}

	void FPSCamera::recomputeMatrices()
	{
		// Calculate the new Front vector
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(mPitchAngle)) * cos(glm::radians(mYawAngle))/* * cos(glm::radians(mRollAngle))*/;
		newFront.y = sin(glm::radians(mPitchAngle))/* * cos(glm::radians(mYawAngle)) * sin(glm::radians(mRollAngle))*/;
		newFront.z = cos(glm::radians(mPitchAngle)) * sin(glm::radians(mYawAngle))/* * cos(glm::radians(mRollAngle))*/;
		mFront = glm::normalize(newFront);
		// Also re-calculate the Right and Up vector
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		mUp = glm::normalize(glm::cross(mRight, mFront));

		mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void FPSCamera::setPosition(const glm::vec3& position)
	{
		mPosition = position;
		recomputeMatrices();
	}

	void FPSCamera::move(const glm::vec3& offset)
	{
		if (offset.x != 0.f) {
			mPosition += offset.x * mRight;
		}

		if (offset.y != 0.f) {
			mPosition += offset.y * mFront;
		}
		
		recomputeMatrices();
	}

	void FPSCamera::setRotationAngle(float angle)
	{
		mRotationAngle = angle;
		mRollAngle = mRotationAngle;
		recomputeMatrices();
	}

	void FPSCamera::rotate(float angleOffset)
	{
		mRotationAngle += angleOffset;
		mRollAngle = mRotationAngle;
		recomputeMatrices();
	}
}
