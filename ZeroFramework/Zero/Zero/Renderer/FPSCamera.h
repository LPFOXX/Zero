#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include "../Core/InputCodes.h"
#include "../Core/Clock.h"
#include "Camera.h"

namespace zr
{
	class FPSCamera : public Camera
	{
	public:
		enum class MovementDirection
		{
			Forward,
			Backward,
			Right,
			Left
		};

		FPSCamera(const glm::vec3& initialPosition, const glm::vec3& front, const glm::vec3& up, float width, float height, float fieldOfView = 45.f);
		virtual ~FPSCamera();

		void processKeyboard(Keyboard key);
		void processMouseMovement(const glm::vec2& mouseOffset);
		void invertMouseMovement(bool invert = true);
		void move(MovementDirection direction, const Time& deltaTime);

		// Inherited via Camera
		virtual void setRotationAngle(float angle) override;
		virtual void rotate(float angleOffset) override;
		virtual void setPosition(const glm::vec3& position) override;
		virtual void move(const glm::vec3& offset) override;

	private:
		// Inherited via Camera
		virtual void recomputeMatrices() override;

	private:
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mWorldUp;
		glm::vec3 mRight;

		float mYawAngle;
		float mPitchAngle;
		float mRollAngle;

		float mFieldOfView;
		bool mIsMouseInverted;
	};
}
