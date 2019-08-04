#pragma once

#include <Zero/Zero.h>

namespace lp
{
	class HUDLayer : public zr::Layer, public zr::ViewportDependable
	{
	public:
		HUDLayer();
		virtual ~HUDLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		std::shared_ptr<zr::OrthographicCamera> mCamera;
		std::shared_ptr<zr::Font> mFont;
		std::shared_ptr<zr::Text> mText;
	};
}
