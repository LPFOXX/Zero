#pragma once

#include <Zero/Zero/Layer.h>

namespace lp
{
	class SandboxLayer : public zr::Layer
	{
	public:
		SandboxLayer();
		virtual ~SandboxLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;
	};
}
