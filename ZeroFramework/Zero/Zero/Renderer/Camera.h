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

		void setPosition(const glm::vec3& position);
		const glm::vec3& getPosition() const;
		void move(const glm::vec3& offset);

		float getRotationAngle() const;
		void setRotationAngle(float angle);
		void rotate(float angleOffset);

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

	private:
		// Inherited via Camera
		virtual void recomputeMatrices() override;
	};
}
