#pragma once

#include <Zero/Zero.h>

#define PROFILE_SCOPE(name) zr::Timer<std::function<void(const zr::ProfileResult&)>> profileTimer##__LINE__(name, [&](const zr::ProfileResult& result) { mProfileResults.push_back(result);})

namespace lp
{
	class EditorLayer : public zr::Layer, public zr::ViewportDependable, public zr::ViewportResizeListener
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		bool drawEditorWindow(zr::Ref<zr::Viewer>& viewer, uint32_t count);
		// Inherited via ViewportResizeListener
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override;
		void AddSceneViewer();

	private:
		/// Viewer Things
		std::vector<zr::Ref<zr::Viewer>> mViewers;
		//zr::Ref<zr::Viewer> mViewer;

		/// Temp things
		std::vector<zr::ProfileResult> mProfileResults;
		zr::Ref<zr::Scene> mCurrentScene;
	};
}
