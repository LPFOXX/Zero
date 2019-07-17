#include <zr_pch.h>

#include "Camera.h"

namespace zr
{
	Camera::Camera() :
		mPosition({0.f, 0.f, 0.f}),
		mViewProjectionMatrix(1.f),
		mProjectionMatrix(1.f),
		mViewMatrix(1.f),
		mRotationAngle(0.f)
	{
	}

	Camera::~Camera()
	{
	}

	const glm::mat4& Camera::getProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	const glm::mat4& Camera::getViewMatrix() const
	{
		return mViewMatrix;
	}

	const glm::mat4& Camera::getViewProjectionMatrix() const
	{
		return mViewProjectionMatrix;
	}

	const glm::vec3& Camera::getPosition() const
	{
		return mPosition;
	}

	float Camera::getRotationAngle() const
	{
		return mRotationAngle;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		Camera()
	{
		mProjectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::recomputeMatrices()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), mPosition) *
			glm::rotate(glm::mat4(1.f), glm::radians(mRotationAngle), glm::vec3(0.f, 0.f, 1.f));

		mViewMatrix = glm::inverse(transform);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthographicCamera::setPosition(const glm::vec3& position)
	{
		mPosition = position;
		recomputeMatrices();
	}

	void OrthographicCamera::move(const glm::vec3& offset)
	{
		mPosition += offset;
		recomputeMatrices();
	}

	void OrthographicCamera::setRotationAngle(float angle)
	{
		mRotationAngle = angle;
		recomputeMatrices();
	}

	void OrthographicCamera::rotate(float angleOffset)
	{
		mRotationAngle += angleOffset;
		recomputeMatrices();
	}

	PerspectiveCamera::PerspectiveCamera(float fieldOfView, float width, float height) :
		Camera()
	{
		float aspectRatio = 1.f;
		if (height != 0) {
			aspectRatio = width / height;
		}

		mProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, -1.f, 1.f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::recomputeMatrices()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), mPosition) *
			glm::rotate(glm::mat4(1.f), glm::radians(mRotationAngle), glm::vec3(0.f, 0.f, 1.f));

		mViewMatrix = glm::lookAt(mPosition, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });

		//mViewMatrix = glm::inverse(transform);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void PerspectiveCamera::setPosition(const glm::vec3& position)
	{
		mPosition = position;
		recomputeMatrices();
	}

	void PerspectiveCamera::move(const glm::vec3& offset)
	{
		mPosition += offset;
		recomputeMatrices();
	}

	void PerspectiveCamera::setRotationAngle(float angle)
	{
		mRotationAngle = angle;
		recomputeMatrices();
	}

	void PerspectiveCamera::rotate(float angleOffset)
	{
		mRotationAngle += angleOffset;
		recomputeMatrices();
	}
}
