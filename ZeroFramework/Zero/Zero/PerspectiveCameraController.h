
#include "Core/Core.h"
#include "Core/Clock.h"
#include "Core/Events.h"
#include "Renderer/Camera.h"
#include "CameraController.hpp"

namespace zr
{
	class PerspectiveCameraController : public CameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio, bool rotation = false);
		virtual ~PerspectiveCameraController();

		void onUpdate(const Time& elapsedTime) override;
		void onEvent(Event& e) override;

		virtual const Ref<Camera>& getCamera() const override;
		virtual const Ref<InputMapper16>& getInputMapper() const override;
		virtual void enableRotation(bool enabled) override;

		void setFieldOfView(float angle);
		float getFieldOfView() const;

	private:
		bool onMouseScrolled(MouseScrollEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
		void readjustCameraTranslationSpeed();

	private:
		float mAspectRatio;
		float mFieldOfView;
		bool mRotation;
		Ref<Camera> mCamera;

		glm::vec3 mCameraPosition;
		float mCameraRotation;
		float mCameraRotationSpeed;
		float mCameraTranslationSpeed;

		Ref<InputMapper16> mInputMapper;
	};
}
