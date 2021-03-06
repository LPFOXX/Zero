#include <zr_pch.h>

#include "Zero/Core/Profiller.h"
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
		setProjection(left, right, bottom, top);
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
	{
		ZR_PROFILER_FUNCTION();

		mProjectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthographicCamera::recomputeMatrices()
	{
		ZR_PROFILER_FUNCTION();

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

	void OrthographicCamera::setSize(float width, float height)
	{
		mProjectionMatrix = glm::ortho(0.f, width, 0.f, height, -1.f, 1.f);
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

	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fieldOfView) :
		Camera()
	{
		mProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, -1.f, 1.f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::setProjection(float aspectRatio, float fieldOfView)
	{
		mProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, -1.f, 1.f);
		recomputeMatrices();
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

	void PerspectiveCamera::setFieldOfView(float angle)
	{
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
