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
		mSprite(nullptr),
		mGame(nullptr)
	{
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

		std::shared_ptr<zr::Texture2D> texture;
		texture = zr::Texture2D::Create();
		texture->loadFromFile("resources/amityafflictionoceancd.jpg", true);
		texture->setSmooth(true);

		mSprite.reset(new zr::Sprite);
		mSprite->setTexture(texture);

		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mSprite));
		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mGame));
	}

	HUDLayer::~HUDLayer()
	{

	}

	void HUDLayer::onAttach()
	{
		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());
	}

	void HUDLayer::onDetach()
	{
	}

	void HUDLayer::onUpdate(const zr::Time& elapsedTime)
	{
		mCameraController->onUpdate(elapsedTime);
		mText->setString(std::to_string(elapsedTime.asMilliseconds()));

		/*if (zr::Input::isKeyPressed(zr::Keyboard::W)) {
			mCamera->move({ 0.f, elapsedTime.asSeconds() * 100.f, 0.f });
		}
		if (zr::Input::isKeyPressed(zr::Keyboard::S)) {
			mCamera->move({ 0.f, -elapsedTime.asSeconds() * 100.f, 0.f });
		}
		if (zr::Input::isKeyPressed(zr::Keyboard::A)) {
			mCamera->move({ -elapsedTime.asSeconds() * 100.f, 0.f, 0.f });
		}
		if (zr::Input::isKeyPressed(zr::Keyboard::D)) {
			mCamera->move({ elapsedTime.asSeconds() * 100.f, 0.f, 0.f });
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Q)) {
			mCamera->rotate(45.f * elapsedTime.asSeconds());
		}
		if (zr::Input::isKeyPressed(zr::Keyboard::E)) {
			mCamera->rotate(-45.f * elapsedTime.asSeconds());
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Up)) {
			mText->move(0.f, elapsedTime.asSeconds() * 100.f);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Down)) {
			mText->move(0.f, -elapsedTime.asSeconds() * 100.f);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Left)) {
			mText->move(-elapsedTime.asSeconds() * 100.f, 0.f);
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Right)) {
			mText->move(elapsedTime.asSeconds() * 100.f, 0.f);
		}*/

		mGame->update();

		zr::Renderer2D::BeginScene(mCameraController->getCamera());
		{
			zr::RenderCommand::SetClearColor(0.f, 0.f, 0.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

			zr::Renderer2D::DrawQuad({ 0.f, 0.f }, { 1.f, 1.f }, { .8f, .2f, .3f, 1.f });
			zr::Renderer2D::EndScene();
		}
		////zr::Renderer::Submit(mSprite);
		//zr::Renderer::Submit(mText);
		//mGame->draw(mCameraController->getCamera()->getViewProjectionMatrix());
	}

	void HUDLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");
		{
			static glm::vec2 spritePosition(0.f, 0.f);
			if (ImGui::DragFloat2("Sprite Position", &spritePosition[0])) {
				mSprite->setPosition(spritePosition);
			}

			static glm::vec2 fpsTextPosition(mText->getPosition());
			if (ImGui::DragFloat2("FPS text Position", &fpsTextPosition[0])) {
				mText->setPosition(fpsTextPosition);
			}

			ImGui::End();
		}
	}

	void HUDLayer::onEvent(zr::Event& e)
	{
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
		ZR_INFO("[HUDLayer] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		notify(viewportSize);
	}
}
