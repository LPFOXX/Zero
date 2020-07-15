#pragma once

#include <Zero/Zero.h>

#define PROFILE_SCOPE(name) zr::Timer<std::function<void(const zr::ProfileResult&)>> profileTimer##__LINE__(name, [&](const zr::ProfileResult& result) { mProfileResults.push_back(result);})

namespace lp
{
	class HUDLayer : public zr::Layer, public zr::ViewportDependable, public zr::ViewportResizeListener
	{
	public:
		HUDLayer();
		virtual ~HUDLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		/// Viewer Things
		zr::Ref<zr::Viewer> mViewer;

		/// Temp things
		std::shared_ptr<zr::Font> mFont;
		std::shared_ptr<zr::Text> mText;
		zr::Ref<zr::Texture2D> mCheckerBoardTexture;
		zr::Ref<zr::MultiLine> mMultiLine;
		
		glm::vec2 mA = glm::vec2(0.f, 5.f);
		glm::vec2 mB = glm::vec2(-4.8f, 1.5f);
		glm::vec2 mC = glm::vec2(-3.f, -4.05f);
		glm::vec2 mD = glm::vec2( 3.f, -4.05f);
		glm::vec2 mE = glm::vec2(4.80f, 1.5f);
		/*glm::vec2 mF = glm::vec2(1.21f, 1.80f);
		glm::vec2 mG = glm::vec2(-2.05f, -2.00f);
		glm::vec2 mH = glm::vec2(-1.f, 2.0f);*/

		std::vector<zr::ProfileResult> mProfileResults;

		glm::vec2 mPoint1 = glm::vec2(0.f, 0.f);
		bool mAutomatePoint1 = false;
		bool mAutomaticC = false;
		float mLength = 5.f;

		zr::Quad mQuad;
		zr::Sprite2D mSprite2D;

		// Lua things
		zr::LuaVM mLuaVM;

		// Inherited via ViewportResizeListener
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override;
	};
}
