#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class Camera
	{
	public:
		Camera();
		virtual ~Camera();

		const glm::mat4& getProjectionMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getViewProjectionMatrix() const;

		const glm::vec3& getPosition() const;
		float getRotationAngle() const;

		virtual void setPosition(const glm::vec3& position) = 0;
		virtual void move(const glm::vec3& offset) = 0;

		virtual void setRotationAngle(float angle) = 0;
		virtual void rotate(float angleOffset) = 0;

	protected:
		virtual void recomputeMatrices() = 0;

	protected:
		glm::vec3 mPosition;
		glm::mat4 mViewProjectionMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		float mRotationAngle;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		virtual ~OrthographicCamera();

		// Inherited via Camera
		virtual void setRotationAngle(float angle) override;
		virtual void rotate(float angleOffset) override;
		virtual void setPosition(const glm::vec3& position) override;
		virtual void move(const glm::vec3& offset) override;

	private:
		// Inherited via Camera
		virtual void recomputeMatrices() override;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fieldOfView, float width, float height);
		virtual ~PerspectiveCamera();

		// Inherited via Camera
		virtual void setRotationAngle(float angle) override;
		virtual void rotate(float angleOffset) override;
		virtual void setPosition(const glm::vec3& position) override;
		virtual void move(const glm::vec3& offset) override;

	private:
		// Inherited via Camera
		virtual void recomputeMatrices() override;
	};
}
