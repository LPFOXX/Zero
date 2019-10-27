
#include "Core/Core.h"
#include "Core/Clock.h"
#include "Core/Events.h"
#include "Renderer/Camera.h"

namespace zr
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio, bool rotation = false);
		virtual ~PerspectiveCameraController();

		void onUpdate(const Time& elapsedTime);
		void onEvent(Event& e);

		const Ref<PerspectiveCamera>& getCamera() const;

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
		Ref<PerspectiveCamera> mCamera;

		glm::vec3 mCameraPosition;
		float mCameraRotation;
		float mCameraRotationSpeed;
		float mCameraTranslationSpeed;
	};
}
