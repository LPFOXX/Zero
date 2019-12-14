#include <pch.h>

#include <numeric>

#include "HUDLayer.h"

namespace lp
{
	static unsigned size = 25;

	HUDLayer::HUDLayer() :
		zr::Layer("HUDLayer"),
		mCameraController(nullptr),
		mFont(nullptr),
		mText(nullptr),
		mGame(nullptr)
	{
		ZR_PROFILER_FUNCTION();
	}

	HUDLayer::~HUDLayer()
	{
		ZR_PROFILER_FUNCTION();
	}

	void HUDLayer::onAttach()
	{
		ZR_PROFILER_FUNCTION();

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

		mFont.reset(new zr::Font);
		if (!mFont->loadFromFile("resources/fonts/futura_book_font.ttf")) {
			ZR_ERROR("[HUDLayer] Can't load HUD font.");
		}

		mText.reset(new zr::Text);
		mText->setFont(mFont);
		mText->setFontSize(12U);
		mText->setFillColor({ 1.f, 1.f, 0.f, 1.f });
		mText->setOutlineColor({ 1.f, 1.f, 1.f, 1.f });
		mText->setPosition(0U, 600U);
		mText->setOutlineThickness(1U);
		std::string textString = R"(
			Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras dolor ipsum,
			sollicitudin sed imperdiet vel, eleifend quis augue. Cras tortor erat, laoreet
			eget dui ut, ullamcorper cursus enim. Donec scelerisque suscipit neque a auctor.
			Vivamus in urna hendrerit, tempor nibh eget, condimentum ligula. Mauris semper
			massa vitae semper elementum. Ut lacus ipsum, sodales eget nibh a, tincidunt
			cursus dolor. Nam erat lacus, efficitur ut nisi id, mattis suscipit turpis.
			In rutrum, massa ac imperdiet tristique, eros ante tristique nibh, a ultricies
			nibh mi vel felis. Praesent massa ligula, gravida ac aliquet non, semper a risus.
			Fusce dui metus, varius id tellus vel, interdum placerat erat. Vestibulum eu felis lorem.

			In laoreet lacus eget vehicula sollicitudin. Phasellus consectetur augue nec libero molestie
			tempus. Morbi dignissim interdum laoreet. Ut sed nunc a nibh vestibulum pharetra. Cras sit
			amet laoreet enim, ac suscipit diam. Sed scelerisque, libero nec pulvinar fringilla, nisl
			nisl suscipit tellus, in gravida sapien nisl quis nunc. Praesent eget nisl nulla. Nam
			ultricies neque at leo mattis, vitae auctor ante tristique. Nam nec dolor sem.

			Aliquam blandit ipsum ac orci iaculis vehicula non sit amet purus. Ut faucibus lacinia
			nisi, eget feugiat turpis semper nec. Proin ultrices nisl est, vel mollis quam volutpat
			nec. Sed fringilla iaculis fringilla. Phasellus feugiat metus libero, et elementum lorem
			gravida sit amet. Sed sit amet eleifend libero, eu accumsan risus. Duis cursus, est blandit
			tincidunt efficitur, lorem massa hendrerit quam, nec lacinia eros ligula id orci. Vestibulum
			in ipsum justo.

			Maecenas convallis, neque sollicitudin pellentesque feugiat, massa felis pellentesque nunc,
			ut malesuada enim lacus vestibulum tellus. Quisque eget dapibus enim. Fusce id malesuada neque.
			Curabitur quis blandit nunc. Ut dignissim, lorem congue rutrum placerat, nisl purus fringilla
			tortor, a lacinia dui ex sed ante. Mauris placerat, lorem ut sagittis vestibulum, nisl risus
			feugiat lectus, eu hendrerit massa ipsum vel quam. Etiam sit amet augue vitae lacus maximus
			ultrices ut a augue. Vestibulum sit amet laoreet tortor. Sed lectus elit, vestibulum nec porta
			ut, euismod vel purus. Suspendisse nec aliquet ante. Pellentesque id nunc non purus venenatis
			laoreet. Ut ac ex ac ante malesuada mollis eget condimentum ex. Vivamus placerat pulvinar lacus,
			id accumsan diam lacinia ut. Maecenas suscipit facilisis lacus, id accumsan mauris pellentesque
			non. Pellentesque id malesuada libero, id imperdiet justo. Etiam at elit sit amet orci ultrices
			pellentesque non a dolor.

			Mauris efficitur risus et congue elementum. Donec sodales lacus neque, sed ultricies massa feugiat
			non. Aenean quam ipsum, malesuada non vestibulum ac, euismod in nibh. Donec placerat est sapien.
			Pellentesque vehicula mi sapien, nec pretium ex egestas quis. Pellentesque quis quam et risus
			rhoncus lobortis at ut augue. Aliquam feugiat ut diam vitae luctus.
		)";
		mText->setString(textString);

		mCameraController.reset(new zr::OrthographicCameraController(1280.f / 600.f, true));
		mGame.reset(new PongGame);

		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mGame));

		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());
	}

	void HUDLayer::onDetach()
	{
		ZR_PROFILER_FUNCTION();
	}

	void HUDLayer::onUpdate(const zr::Time& elapsedTime)
	{
		ZR_PROFILER_FUNCTION();
		PROFILE_SCOPE("HUDLayer::onUpdate");
		{
			PROFILE_SCOPE("CameraController::onUpdate");
			mCameraController->onUpdate(elapsedTime);
		}

		mGame->update();

		zr::RenderCommand::SetClearColor(0.f, 0.f, 0.f, 1.f);
		zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);

		{
			ZR_PROFILER_SCOPE("Renderer Draw");
			zr::Renderer2D::BeginScene(mCameraController->getCamera());
			{
				const zr::Time& time = zr::Application::GetTime();
				{
					PROFILE_SCOPE("A bunch of quads");
					for (unsigned i = 0; i < size; ++i) {
						for (unsigned j = 0; j < size; ++j) {
							glm::vec4 quadColor(float(i) / size, float(j) / size, ((float(i) / size) + (float(j) / size)) / 2.f, 1.f);
							//float rotation = time.asSeconds() + (float(i) * 45.f) + (float(j) * 90.f);
							zr::Renderer2D::DrawQuad({ i, j }, { .9f, .9f }, quadColor);
						}
					}
				}

				{
					PROFILE_SCOPE("Draw 2 Quads");
					zr::Renderer2D::DrawRotatedQuad({ 0.5f, -.5f }, { .5f, .75f }, -time.asSeconds() * 30.f, { .2f, .3f, .8f, 1.f });
					zr::Renderer2D::DrawRotatedQuad({ -1.f, 0.f }, { .8f, .8f }, time.asSeconds() * 45.f, { .8f, .2f, .3f, 1.f });
				}

				{
					PROFILE_SCOPE("Draw 2 Textures");
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.2f }, { 10.f, 10.f }, mCheckerBoardTexture, { 20.f, 20.f }, { .2f, .3f, .8f, .5f });
					//zr::Renderer2D::DrawQuad({ .0f, .0f, -.15f }, { 10.f, 10.f }, 0, mOceanTexture, { 10.f, 10.f }, { 1.f, 1.f, 1.f, 1.f });
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, mLogoTexture, { 20.f, 20.f }, { .2f, .3f, .8f, .5f });
				}

				/*{
					PROFILE_SCOPE("Draw batched text");
					mText->render(mCameraController->getCamera()->getViewProjectionMatrix());
				}*/
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, -time.asSeconds() * 45.f, mTexture, {20.f, 20.f}, { abs(sin(time.asSeconds() * .5f)), abs(cos(time.asSeconds() * .1f)), abs(sin(time.asSeconds()) * cos(time.asSeconds())), 1.f });
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, 0, mTexture, {20.f, 20.f}, { 1.f, 1.f, 1.f, 1.f});

				zr::Renderer2D::EndScene();
			}
		}
		////zr::Renderer::Submit(mSprite);
		//mGame->draw(mCameraController->getCamera()->getViewProjectionMatrix());
	}

	void HUDLayer::OnImGuiRender()
	{
		ZR_PROFILER_FUNCTION();
		ImGui::Begin("Settings");
		{
			static glm::vec2 fpsTextPosition(mText->getPosition());
			if (ImGui::DragFloat2("FPS text Position", &fpsTextPosition[0])) {
				mText->setPosition(fpsTextPosition);
			}


			ImGui::DragInt("Size", reinterpret_cast<int*>(&size), 1.f, 1, 2000000);
			ImGui::Text("%i vertices", size * size * 4);

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
		ZR_PROFILER_FUNCTION();
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
		ZR_PROFILER_FUNCTION();
		ZR_INFO("[HUDLayer] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
		notify(viewportSize);
	}
}
