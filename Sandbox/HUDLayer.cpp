#include <pch.h>

#include "HUDLayer.h"

namespace lp
{
	HUDLayer::HUDLayer() :
		zr::Layer("HUDLayer"),
		mCamera(nullptr),
		mFont(nullptr),
		mText(nullptr)
	{
		mCamera.reset(new zr::OrthographicCamera(0.f, 1280.f, 0.f, 600.f));
		mFont.reset(new zr::Font);
		if (!mFont->loadFromFile("resources/fonts/cutive_mono_regular.ttf")) {
			ZR_ERROR("[HUDLayer] Can't load HUD font.");
		}

		mText.reset(new zr::Text);
		mText->setFont(mFont);
		mText->setFontSize(24U);
		mText->setFillColor({ 0.f, 1.f, 0.f, 1.f });

		mText->setPosition({ 2U, 2U });
		mText->setOutlineThickness(2U);
		mText->setOutlineColor({ 0.f, 0.f, 0.f, 1.f });

		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
	}

	HUDLayer::~HUDLayer()
	{

	}

	void HUDLayer::onAttach()
	{
		zr::Window& window = zr::Application::GetInstance().getWindow();
		viewportUpdate(window.getWidth(), window.getHeight());
	}

	void HUDLayer::onDetach()
	{
	}

	void HUDLayer::onUpdate(const zr::Time& elapsedTime)
	{
		mText->setString(std::to_string(elapsedTime.asMilliseconds()));

		if (zr::Input::isKeyPressed(zr::Keyboard::W)) {
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
		}

		zr::Renderer::BeginScene(mCamera);
		{
			zr::RenderCommand::SetClearColor(1.f, 1.f, 0.f, 1.f);
			zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);
			zr::Renderer::Submit(mText);
		}
		zr::Renderer::EndScene();
	}

	void HUDLayer::OnImGuiRender()
	{
	}

	void HUDLayer::onEvent(zr::Event& e)
	{
		if (e.getType() == zr::EventType::WindowResize) {
			const zr::WindowResizeEvent& resizeEvent = dynamic_cast<zr::WindowResizeEvent&>(e);
			mCamera->setSize(resizeEvent.getWidth(), resizeEvent.getHeight());
			viewportUpdate(resizeEvent.getWidth(), resizeEvent.getHeight());
		}

		if (e.getType() == zr::EventType::KeyPressed) {
			const zr::KeyPressedEvent& keyEvent = dynamic_cast<zr::KeyPressedEvent&>(e);

			if (keyEvent.getKeyCode() == static_cast<int>(zr::Keyboard::P)) {
				unsubscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
			}
		}
	}
}
