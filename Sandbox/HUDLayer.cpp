#include <pch.h>

#include <numeric>

#include "HUDLayer.h"

namespace lp
{
	HUDLayer::HUDLayer() :
		zr::Layer("HUDLayer"),
		mCameraController(nullptr),
		mFont(nullptr),
		mText(nullptr),
		mGame(nullptr)
	{
		PROFILER_FUNCTION();
		mCameraController.reset(new zr::OrthographicCameraController(1280.f / 600.f, true));
		mFont.reset(new zr::Font);
		if (!mFont->loadFromFile("resources/fonts/futura_book_font.ttf")) {
			ZR_ERROR("[HUDLayer] Can't load HUD font.");
		}

		mGame.reset(new PongGame);

		mText.reset(new zr::Text);
		mText->setFont(mFont);
		mText->setFontSize(24U);
		mText->setFillColor({ 0.f, 1.f, 0.f, 1.f });
		mText->setPosition(2U, 2U);
		mText->setOutlineThickness(2U);
		mText->setOutlineColor({ 0.f, 0.f, 0.f, 1.f });
		mText->setString("Lidneu$");

		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mGame));
	}

	HUDLayer::~HUDLayer()
	{
		PROFILER_FUNCTION();
	}

	void HUDLayer::onAttach()
	{
		PROFILER_FUNCTION();
		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());

		mCheckerBoardTexture = zr::Texture2D::Create();
		mCheckerBoardTexture->loadFromFile("resources/textures/Checkerboard.png", true);
		mCheckerBoardTexture->setRepeat(true);
		mCheckerBoardTexture->generateMipMaps();

		mLogoTexture = zr::Texture2D::Create();
		mLogoTexture->loadFromFile("resources/textures/ChernoLogo.png", true);
		mLogoTexture->setRepeat(true);
		mLogoTexture->generateMipMaps();

		mOceanTexture = zr::Texture2D::Create();
		mOceanTexture->loadFromFile("resources/amityafflictionoceancd.jpg", true);
		mOceanTexture->setSmooth(true);
		mOceanTexture->setRepeat(true);
		mOceanTexture->generateMipMaps();
	}

	void HUDLayer::onDetach()
	{
		PROFILER_FUNCTION();
	}

	void HUDLayer::onUpdate(const zr::Time& elapsedTime)
	{
		PROFILER_FUNCTION();
		PROFILE_SCOPE("HUDLayer::onUpdate");
		{
			PROFILER_SCOPE("CameraController::onUpdate");
			PROFILE_SCOPE("CameraController::onUpdate");
			mCameraController->onUpdate(elapsedTime);
		}

		{
			PROFILER_SCOPE("PongGame::onUpdate");
			PROFILE_SCOPE("PongGame::onUpdate");
			mGame->update();
		}

		{
			//PROFILER_SCOPE("Renderer Prep");
			PROFILE_SCOPE("Renderer Prep");
			zr::RenderCommand::SetClearColor(0.f, 0.f, 0.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);
		}

		{
			//PROFILER_SCOPE("Renderer Draw");
			PROFILE_SCOPE("Renderer Draw");
			zr::Renderer2D::BeginScene(mCameraController->getCamera());
			{
				const zr::Time& time = zr::Application::GetTime();

				/*for (unsigned i = 0; i < 5; ++i) {
					for (unsigned j = 0; j < 5; ++j) {
						glm::vec4 quadColor(float(i) / 5.f, float(j) / 5.f, ((float(i) / 5.f) + (float(j) / 5.f)) / 2.f, 1.f);
						float rotation = time.asSeconds() + (float(i) * 45.f) + (float(j) * 90.f);
						zr::Renderer2D::DrawQuad({ i, j }, { .9f, .9f }, rotation, quadColor);
					}
				}*/

				{
					PROFILER_SCOPE("Draw Single Quad1");
					PROFILE_SCOPE("Draw Single Quad1");
					zr::Renderer2D::DrawQuad({ -1.f, 0.f }, { .8f, .8f }, time.asSeconds() * 45.f, { .8f, .2f, .3f, 1.f });
				}

				{
					PROFILER_SCOPE("Draw Single Quad2");
					PROFILE_SCOPE("Draw Single Quad2");
					zr::Renderer2D::DrawQuad({ 0.5f, -.5f }, { .5f, .75f }, -time.asSeconds() * 30.f, { .2f, .3f, .8f, 1.f });
				}

				{
					PROFILER_SCOPE("Draw CheckerBoard Texture");
					PROFILE_SCOPE("Draw CheckerBoard Texture");
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.2f }, { 10.f, 10.f }, 0, mCheckerBoardTexture, { 20.f, 20.f }, { .2f, .3f, .8f, .5f });
				}

				{
					PROFILER_SCOPE("Draw Ocean Texture");
					PROFILE_SCOPE("Draw Ocena Texture");
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.15f }, { 10.f, 10.f }, 0, mOceanTexture, { 10.f, 10.f }, { 1.f, 1.f, 1.f, 1.f });
				}

				{
					PROFILER_SCOPE("Draw Logo Texture");
					PROFILE_SCOPE("Draw Logo Texture");
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, 0, mLogoTexture, { 20.f, 20.f }, { .2f, .3f, .8f, .5f });
				}
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, -time.asSeconds() * 45.f, mTexture, {20.f, 20.f}, { abs(sin(time.asSeconds() * .5f)), abs(cos(time.asSeconds() * .1f)), abs(sin(time.asSeconds()) * cos(time.asSeconds())), 1.f });
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, 0, mTexture, {20.f, 20.f}, { 1.f, 1.f, 1.f, 1.f});

				zr::Renderer2D::EndScene();
			}
		}
		////zr::Renderer::Submit(mSprite);
		//zr::Renderer::Submit(mText);
		//mText->render(mCameraController->getCamera()->getViewProjectionMatrix());
		//mGame->draw(mCameraController->getCamera()->getViewProjectionMatrix());
	}

	void HUDLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");
		{
			static glm::vec2 fpsTextPosition(mText->getPosition());
			if (ImGui::DragFloat2("FPS text Position", &fpsTextPosition[0])) {
				mText->setPosition(fpsTextPosition);
			}

			for (auto& result : mProfileResults) {
				char label[50];
				strcpy(label, "%.3fms ");
				strcat(label, result.Name);
				ImGui::Text(label, result.Time.asMilliseconds());
			}
			mProfileResults.clear();

			ImGui::End();
		}
	}

	void HUDLayer::onEvent(zr::Event& e)
	{
		PROFILER_FUNCTION();
		mCameraController->onEvent(e);

		if (e.getType() == zr::EventType::WindowResize) {
			const zr::WindowResizeEvent& resizeEvent = dynamic_cast<zr::WindowResizeEvent&>(e);
			//mCameraController->getCamera()->setSize(resizeEvent.getWidth(), resizeEvent.getHeight());
			viewportUpdate(resizeEvent.getWidth(), resizeEvent.getHeight());
		}

		if (e.getType() == zr::EventType::KeyPressed) {
			const zr::KeyPressedEvent& keyEvent = dynamic_cast<zr::KeyPressedEvent&>(e);

			if (keyEvent.getKeyCode() == (int)zr::Keyboard::Space) {
				mGame->reset();
			}
		}
	}

	void HUDLayer::onViewportUpdate(const glm::vec2& viewportSize)
	{
		PROFILER_FUNCTION();
		ZR_INFO("[HUDLayer] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		notify(viewportSize);
	}
}
