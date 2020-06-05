#pragma once

#include "Core/Core.h"
#include "Core/Clock.h"
#include "Core/Events.h"
#include "Core/Input.h"

namespace zr
{
	class CameraController
	{
	public:
		enum Actions : uint16_t
		{
			MoveCameraRight,
			MoveCameraLeft,
			MoveCameraUp,
			MoveCameraDown,

			RotateCameraRight,
			RotateCameraLeft
		};

	public:
		CameraController(){ }
		virtual ~CameraController() = default;

		virtual void onUpdate(const Time& elapsedTime) = 0;
		virtual void onEvent(Event& e) = 0;

		virtual const Ref<Camera>& getCamera() const = 0;
		virtual const Ref<InputMapper16>& getInputMapper() const = 0;
		virtual void enableRotation(bool enabled) = 0;
	};
}
