#include <pch.h>

#include "HUDLayer.h"
#include "Zero/vendor/imgui/imgui_internal.h"

namespace lp
{
	static unsigned size = 25;

	void editViewSizeCallback(ImGuiSizeCallbackData* data)
	{
		ZR_IMGUI_CONSOLE_INFO("Window Current Size (%.2f, %.2f) -> Window Desired Size (%.2f, %.2f)", data->CurrentSize.x, data->CurrentSize.y, data->DesiredSize.x, data->DesiredSize.y);
	}

	HUDLayer::HUDLayer() :
		zr::Layer("HUDLayer"),
		mCameraController(nullptr),
		mEditViewCanReceiveInput(false),
		mFont(nullptr),
		mText(nullptr)
		//mGame(nullptr)
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

		std::cout << std::boolalpha;
		std::cout << "zr::Font is abstract: " << std::is_abstract<zr::Font>::value << "\n";
		std::cout << "zr::Texture2D is abstract: " << std::is_abstract<zr::Texture2D>::value << "\n";

		const auto& handler1 = zr::AssetManager<zr::Font>::Get().load("resources/fonts/futura_book_font.ttf");
		const auto& handler2 = zr::AssetManager<zr::Texture2D>::Get().load("resources/textures/Checkerboard.png");
		const auto& handler3 = zr::AssetManager<zr::Shader>::Get().load("resources/shaders/Texture.glsl");
		//const auto& handler4 = zr::AssetManager<zr::Model3D>::Get().load("resources/models/nanosuit.obj");

		const auto& handler5 = zr::AssetManager<zr::Framebuffer>::Get().create("MainFramebuffer", zr::Framebuffer::FramebufferProperties { 1280, 720, 8 });

		if (!mLuaVM.loadFile("resources/scripts/Layer.lua")) {
			ZR_IMGUI_CONSOLE_ERROR("Unable to load Lua script file.");
		}

		if (!mLuaVM.runFunction("onAttach", 0)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua funtion.");
		}

		mQuad.setPosition(0.f, 0.f);
		//mQuad.setColor({ 1.f, 1.f, 0.f, 1.f });
		mQuad.setSize(50.f, 50.f);
		mQuad.setColor(
			{ 1.f, 0.f, 0.f, 1.f },		// Red
			{ 0.f, 1.f, 0.f, 1.f },		// Green
			{ 0.f, 0.f, 1.f, 1.f },		// Blue
			{ 1.f, 1.f, 0.f, 1.f });	// Yellow

		mSprite2D.setPosition(0.f, 0.f);
		mSprite2D.setSize(25.f, 25.f);
		// mSprite2D.setTexture();
		mQuad.setSize(25.f, 25.f);
		
		mFramebuffer = zr::Framebuffer::Create({ 1280, 720, 8 });

		/*for (unsigned i = 3; i <= 27; ++i) {
			zr::Ref<zr::RegularShape> shape = zr::CreateRef<zr::RegularShape>(i);
			shape->setFillColor({ std::abs(std::sin(.8f * i)), .8f, std::abs(std::cos(.3f * i)), 1.f });
			shape->setOutlineThickness(2.f);
			shape->setOutlineColor({ std::abs(std::cos(.8f * i)), .8f, std::abs(std::sin(.3f * i)), 1.f });
			mShapes.emplace_back(shape);
		}*/

		mCheckerBoardTexture = zr::Texture2D::Create();
		mCheckerBoardTexture->loadFromFile("resources/textures/Checkerboard.png", true);
		mCheckerBoardTexture->setRepeat(true);
		mCheckerBoardTexture->generateMipMaps();

		/*mLogoTexture = zr::Texture2D::Create();
		mLogoTexture->loadFromFile("resources/textures/ChernoLogo.png", true);
		mLogoTexture->setRepeat(true);
		mLogoTexture->generateMipMaps();*/

		/*mOceanTexture = zr::Texture2D::Create();
		mOceanTexture->loadFromFile("resources/amityafflictionoceancd.jpg", true);
		mOceanTexture->setSmooth(true);
		mOceanTexture->setRepeat(true);
		mOceanTexture->generateMipMaps();*/

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

		float windowWidth = (float)zr::Application::GetWindow().getWidth();
		float windowHeight = (float)zr::Application::GetWindow().getHeight();
		mCameraController.reset(new zr::OrthographicCameraController(windowWidth / windowHeight, true));
		//mGame.reset(new PongGame);

		/*mLine = zr::CreateRef<zr::Line>();
		mLine->setThickness(1.f);
		mLine->setPoints({ 0.f, 0.f }, { 0.f, 0.f });
		mLine->setFillColor({ 1.f, 1.f, 0.f, 1.f });*/

		mMultiLine = zr::CreateRef<zr::MultiLine>();
		mMultiLine->setThickness(.07f);
		//mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });
		mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
		mMultiLine->setFillColor({ 0.f, 1.f, 1.f, 1.f });

		this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mText));
		//this->subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mGame));

		zr::Window& window = zr::Application::GetWindow();
		viewportUpdate(window.getWidth(), window.getHeight());
	}

	void HUDLayer::onDetach()
	{
		ZR_PROFILER_FUNCTION();

		if (!mLuaVM.runFunction("onDetach", 0)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}
	}

	void HUDLayer::onUpdate(const zr::Time& elapsedTime)
	{
		ZR_PROFILER_FUNCTION();
		if (!mLuaVM.runFunction("onUpdate", 0, elapsedTime)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}

		PROFILE_SCOPE("HUDLayer::onUpdate");
		{
			PROFILE_SCOPE("CameraController::onUpdate");
			if (mEditViewCanReceiveInput) {
				mCameraController->onUpdate(elapsedTime);
			}
		}

		//mGame->update();

		mFramebuffer->bind();
		zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color | zr::RendererAPI::ClearBuffers::Depth);
		zr::RenderCommand::SetClearColor(.3f, .3f, .8f, 1.f);

		{
			ZR_PROFILER_SCOPE("Renderer Draw");
			const zr::Time& time = zr::Application::GetTime();

			/*if (mAutomatePoint1) {
				mPoint1.x = mLength * glm::sin(time.asSeconds());
				mPoint1.y = mLength * glm::cos(time.asSeconds());

				mLine->setPoint1(mPoint1);
			}*/

			if (mAutomaticC) {
				mA.x = mLength * .5f * glm::cos(time.asSeconds());
				mA.y = mLength * .5f * glm::sin(time.asSeconds());

				mB.x = mLength * glm::cos(time.asSeconds() * mLength * .1f);
				mB.y = mLength * glm::sin(time.asSeconds() * mLength * .1f);

				mC.x = mLength * glm::sin(time.asSeconds());
				mC.y = mLength * glm::cos(time.asSeconds());

				mD.x = mLength * .5f * glm::sin(time.asSeconds() * mLength * .5f);
				mD.y = mLength * .5f * glm::cos(time.asSeconds() * mLength * .5f);

				mE.x = mLength * .5f * glm::cos(time.asSeconds() * mLength * .5f);
				mE.y = mLength * .5f * glm::sin(time.asSeconds() * mLength * .5f);

				// mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });
				mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
			}


			zr::Renderer2D::BeginScene(mCameraController->getCamera(), mFramebuffer);
			{
				{
					PROFILE_SCOPE("Draw Background");
					zr::Renderer2D::DrawQuad({ .0f, .0f, -.2f }, { 100.f, 100.f }, mCheckerBoardTexture, { 5.f, 5.f });


					//zr::Renderer2D::DrawQuad({ .0f, .0f, -.15f }, { 10.f, 10.f }, 0, mOceanTexture, { 10.f, 10.f }, { 1.f, 1.f, 1.f, 1.f });
					//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, mLogoTexture, { 10.f, 10.f }, { .2f, .3f, .8f, .5f });
				}

				{
					PROFILE_SCOPE("Draw Quad Class");
					if (zr::Input::isKeyPressed(zr::Keyboard::Up)) {
						mQuad.move(0.f, time.asSeconds() * .1);
					}

					if (zr::Input::isKeyPressed(zr::Keyboard::Down)) {
						mQuad.move(0.f, -time.asSeconds() * .1f);
					}

					if (zr::Input::isKeyPressed(zr::Keyboard::Left)) {
						mQuad.move(-time.asSeconds() * .1f, 0.f);
					}

					if (zr::Input::isKeyPressed(zr::Keyboard::Right)) {
						mQuad.move(time.asSeconds() * .1f, 0.f);
					}

					mQuad.draw();
				}

				//{
				//	PROFILE_SCOPE("A bunch of quads");
				//	for (unsigned i = 0; i < size; ++i) {
				//		for (unsigned j = 0; j < size; ++j) {
				//			glm::vec4 quadColor(float(i) / size, float(j) / size, ((float(i) / size) + (float(j) / size)) / 2.f, 1.f);
				//			//float rotation = time.asSeconds() + (float(i) * 45.f) + (float(j) * 90.f);
				//			zr::Renderer2D::DrawQuad({ i, j }, { 1.f, 1.f }, quadColor);
				//		}
				//	}
				//}

				/*{
					PROFILE_SCOPE("Draw 2 Quads");
					zr::Renderer2D::DrawRotatedQuad({ 0.5f, -.5f }, { .5f, .75f }, -time.asSeconds() * 30.f, { .2f, .3f, .8f, 1.f });
					zr::Renderer2D::DrawRotatedQuad({ -1.f, 0.f }, { .8f, .8f }, time.asSeconds() * 45.f, { .8f, .2f, .3f, 1.f });
				}*/

				{
					PROFILE_SCOPE("Draw MultiLine");
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 1.f, glm::radians(time.asSeconds() * -30.f));
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 1.5f, glm::radians(time.asSeconds() * 30.f));
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 2.f, glm::radians(time.asSeconds() * -40.f));
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 3.f, glm::radians(time.asSeconds() * 40.f));
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 4.f, glm::radians(time.asSeconds() * -60.f));
					zr::Renderer2D::DrawRotatedShape(mMultiLine, { 0.f, 0.f }, 5.5f, glm::radians(time.asSeconds() * 60.f));
				}

				zr::Renderer2D::DrawQuad({ 0.f, 0.f, 1.f }, { 10.f, 10.f }, { .2f, .3f, .8f, .1f });

				/*{
					PROFILE_SCOPE("Draw Shapes");
					for (unsigned i = 0U; i < mShapes.size(); ++i) {
						zr::Renderer2D::DrawRotatedShape(mShapes[i], { i, 0.f }, 1.f, -time.asSeconds() * 15.f);
					}
				}*/

				/*{
					PROFILE_SCOPE("Draw Line");
					zr::Renderer2D::DrawShape(mLine, { 0.f, 0.f }, 1.f);
				}*/

				/*{
					PROFILE_SCOPE("Draw batched text");
					mText->render(mCameraController->getCamera()->getViewProjectionMatrix());
				}*/
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, -time.asSeconds() * 45.f, mTexture, {20.f, 20.f}, { abs(sin(time.asSeconds() * .5f)), abs(cos(time.asSeconds() * .1f)), abs(sin(time.asSeconds()) * cos(time.asSeconds())), 1.f });
				//zr::Renderer2D::DrawQuad({ .0f, .0f, -.1f }, { 10.f, 10.f }, 0, mTexture, {20.f, 20.f}, { 1.f, 1.f, 1.f, 1.f});

				zr::Renderer2D::EndScene();
			}
		}
		zr::Framebuffer::BindDefault();
		zr::RenderCommand::Clear(zr::RendererAPI::ClearBuffers::Color);
		zr::RenderCommand::SetClearColor(.3f, .3f, 0.3f, 1.f);

		//mGame->draw(mCameraController->getCamera()->getViewProjectionMatrix());
	}

	void HUDLayer::onImGuiRender()
	{
		ZR_PROFILER_FUNCTION();

		ImGui::Begin("Settings");
		{
			static glm::vec2 fpsTextPosition(mText->getPosition());
			if (ImGui::DragFloat2("FPS text Position", &fpsTextPosition[0])) {
				mText->setPosition(fpsTextPosition.x, fpsTextPosition.y);
			}

			/*static glm::vec2 point0(0.f, 0.f);
			if (ImGui::DragFloat2("Point 0", &point0.x)) {
				mLine->setPoint0(point0);
			}*/

			/*ImGui::Checkbox("Automatic Point 1", &mAutomatePoint1);
			ImGui::Checkbox("Automatic Points", &mAutomaticC);
			if (ImGui::DragFloat2("Point 1", &mPoint1.x)) {
				mLine->setPoint1(mPoint1);
			}*/

			ImGui::DragFloat("Line Length", &mLength, 0.1f, 0.1f, 5000.f);

			static float lineThickness = 0.07f;
			if (ImGui::DragFloat("Line Thickness", &lineThickness, 0.01f, 0.01f, 5.f)) {
				//mLine->setThickness(lineThickness);
				mMultiLine->setThickness(lineThickness);
			}

			ImGui::DragInt("Size", reinterpret_cast<int*>(&size), 1.f, 1, 2000000);
			ImGui::Text("%i vertices", size * size * 4);

			/*if (ImGui::DragFloat2("A", &mA.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });
			if (ImGui::DragFloat2("B", &mB.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });
			if (ImGui::DragFloat2("C", &mC.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });
			if (ImGui::DragFloat2("D", &mD.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mF, mG, mH });*/
			if (ImGui::DragFloat2("A", &mA.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
			if (ImGui::DragFloat2("B", &mB.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
			if (ImGui::DragFloat2("C", &mC.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
			if (ImGui::DragFloat2("D", &mD.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });
			if (ImGui::DragFloat2("E", &mE.x, 0.1f)) mMultiLine->setVertices({ mA, mB, mC, mD, mE, mA });

			for (auto& result : mProfileResults) {
				char label[50];
				strcpy(label, "%.3fms ");
				strcat(label, result.Name);
				ImGui::Text(label, result.Time.asMilliseconds());
			}
			mProfileResults.clear();

			static float quadDepthLevel = 0.f;
			if (ImGui::DragFloat("Quad Depth Level", &quadDepthLevel, 0.01f)) {
				mQuad.setDepthLevel(quadDepthLevel);
			}

			drawEditorWindow(mFramebuffer, mEditViewCanReceiveInput);

			ImGui::End();
		}
	}

	void HUDLayer::drawEditorWindow(const zr::Ref<zr::Framebuffer>& framebuffer, bool& canReceiveInput)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("View##edit_view", (bool*)false, ImGuiWindowFlags_NoCollapse);
		{
			auto windowFound = ImGui::FindWindowByName("View##edit_view");
			if (windowFound) {
				ImGuiContext* context = ImGui::GetCurrentContext();
				canReceiveInput = context && context->HoveredWindow && windowFound->ID == context->HoveredWindow->ID && !windowFound->Collapsed;
			}
			else {
				canReceiveInput = false;
			}

			ImVec2& pos = ImGui::GetCursorScreenPos();
			ImVec2 pos0 = ImGui::GetWindowContentRegionMin();
			ImVec2 pos1 = ImGui::GetWindowContentRegionMax();

			ImVec2 avail = ImGui::GetContentRegionAvail();
			//std::cout << "avail = " << avail.x << " " << avail.y << "\n";

			auto& framebufferSize = framebuffer->getPixelSize();

			//if (avail.x != framebufferSize.x || avail.y != framebufferSize.y) {
			//	/*mFramebufferSize.x = avail.x;
			//	mFramebufferSize.y = avail.y;*/
			//	zr::WindowResizeEvent e(mFramebufferSize.x, mFramebufferSize.y);
			//	mCameraController->onEvent(e);
			//}

			//ImVec2 size(pos1.x - pos0.x, pos1.y - pos0.y);
			ImVec2 size(pos1.x - pos0.x, pos1.y - pos0.y);

			//ImGui::Text("WindowPosition: (%f, %f)\nPos0: (%f, %f)\nPos1: (%f, %f)", pos.x, pos.y, pos0.x, pos0.y, pos1.x, pos1.y);

			ImGui::GetWindowDrawList()->AddImage((void*)(unsigned*)mFramebuffer->getTextureHandle(), pos, ImVec2(pos.x + size.x, pos.y + size.y), { 0.f, 1.f }, { 1.f, 0.f });
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}

	void HUDLayer::onEvent(zr::Event& e)
	{
		ZR_PROFILER_FUNCTION();
		if (!mLuaVM.runFunction("onEvent", 0, e)) {
			ZR_IMGUI_CONSOLE_ERROR("Can't run lua function.");
		}

		if (e.getType() == zr::EventType::WindowResize) {
			/*zr::WindowResizeEvent ev(mFramebufferSize.x, mFramebufferSize.y);
			mCameraController->onEvent(ev);*/
			mCameraController->onEvent(e);
		}
		else if (mEditViewCanReceiveInput) {
			/*zr::WindowResizeEvent ev(mFramebufferSize.x, mFramebufferSize.y);
			mCameraController->onEvent(ev);*/
			mCameraController->onEvent(e);
		}

		if (e.getType() == zr::EventType::WindowResize) {
		/*	auto& framebufferSize = mFramebuffer->getPixelSize();*/
			const zr::WindowResizeEvent& resizeEvent = dynamic_cast<zr::WindowResizeEvent&>(e);
			//mCameraController->getCamera()->setSize(framebufferSize.x, framebufferSize.y);
			viewportUpdate(resizeEvent.getWidth(), resizeEvent.getHeight());
		}

		if (e.getType() == zr::EventType::KeyPressed) {
			const zr::KeyPressedEvent& keyEvent = dynamic_cast<zr::KeyPressedEvent&>(e);

			if (keyEvent.getKeyCode() == (int)zr::Keyboard::Space) {
				//mGame->reset();
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
