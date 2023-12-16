#include <pch.hpp>

#include "ZeroLuaLayer.hpp"

#include "lua.hpp"

namespace lp
{
	struct Sprite
	{
		Sprite() :
			x(0.f),
			y(0.f)
		{
			std::cout << "Sprite ctor called\n";
		}

		~Sprite()
		{
			std::cout << "Sprite dtor called\n";
		}

		float x, y;
	};

	ZeroLuaLayer::ZeroLuaLayer() :
		zr::Layer("LuaLayer"),
		mLuaVM()
	{
		if (mLuaVM) {
			/*mLuaVM.createClass<Sprite>(
				"Sprite",
				{
					{"move", [](lua_State* L) -> int {
						return 0;
					}},
					{"setSize", [](lua_State* L) -> int {
						return 0;
					}}
				});*/

			/*mLuaVM.createClass<Paddle>(
				"Paddle",
				{
					{"setSpeed", [](lua_State* L)->int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isnumber(L, 2)) {
								Paddle* paddle = (Paddle*)lua_touserdata(L, 1);
								float newSpeed = (float)lua_tonumber(L, 2);

								paddle->setSpeed(newSpeed);
							}
						}
						return 0;
					}},
					{ "getSpeed", [](lua_State* L)->int {
						if (lua_isuserdata(L, 1)) {
							Paddle* paddle = (Paddle*)lua_touserdata(L, 1);
							lua_pushnumber(L, paddle->getSpeed());
							return 1;
						}
						return 0;
					}},
					{ "incrementScore", [](lua_State* L)->int {
						if (lua_isuserdata(L, 1)) {
							Paddle* paddle = (Paddle*)lua_touserdata(L, 1);
							paddle->incrementScore();
						}
						return 0;
					}},
					{ "getMovementDirection", [](lua_State* L)->int {
						if (lua_isuserdata(L, 1)) {
							Paddle* paddle = (Paddle*)lua_touserdata(L, 1);
							lua_pushinteger(L, (int)paddle->getMovementDirection());
							return 1;
						}
						return 0;
					}},
					{ "onViewportUpdate", [](lua_State* L)->int {
						return 0;
					}},
					{ "resetPosition", [](lua_State* L)->int {
						if (lua_isuserdata(L, 1)) {
							Paddle* paddle = (Paddle*)lua_touserdata(L, 1);
							paddle->resetPosition();
						}
						return 0;
					}}
				});*/

			/*mLuaVM.createClass<Ball>(
				"Ball",
				{
					{ "setDirectionX", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isnumber(L, 2)) {
								Ball* ball = (Ball*)lua_touserdata(L, 1);
								ball->setDirectionX((Direction)lua_tonumber(L, 2));
							}
						}
						return 0;
					}},
					{ "setDirectionY", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isnumber(L, 2)) {
								Ball* ball = (Ball*)lua_touserdata(L, 1);
								ball->setDirectionY((Direction)lua_tonumber(L, 2));
							}
						}
						return 0;
					}},
					{ "getDirectionX", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							Ball* ball = (Ball*)lua_touserdata(L, 1);
							lua_pushinteger(L, (int)ball->getMovementDirectionX());
						}
						return 1;
					}},
					{ "getDirectionY", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							Ball* ball = (Ball*)lua_touserdata(L, 1);
							lua_pushinteger(L, (int)ball->getMovementDirectionY());
						}
						return 1;
					}},
					{ "setTarget", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isuserdata(L, 2)) {
								Ball* ball = (Ball*)lua_touserdata(L, 1);
								//* target = (Ball*)lua_touserdata(L, 2);

								//ball->setTarget()
							}
						}
						return 0;
					}},
					{ "getSpeed", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							Ball* ball = (Ball*)lua_touserdata(L, 1);
							lua_pushnumber(L, ball->getSpeed());
						}
						return 1;
					}},
					{ "move", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
								Ball* ball = (Ball*)lua_touserdata(L, 1);
								//ball->move();
							}
						}
						return 0;
					}},
					{ "reset", [](lua_State* L) -> int {
						if (lua_isuserdata(L, 1)) {
							if (lua_isnumber(L, 2)) {
								Ball* ball = (Ball*)lua_touserdata(L, 1);
								ball->reset((float)lua_tonumber(L, 2));
							}
						}
						return 0;
					}}
				});*/

			if (mLuaVM.loadFile("resources/scripts/Pong.lua")) {
				std::cout << "File load successfully\n";
			}
			else {
				std::cout << "Can't load file\n";
			}
		}
	}

	ZeroLuaLayer::~ZeroLuaLayer()
	{
	}

	void ZeroLuaLayer::onAttach()
	{
		if (mLuaVM) {
			mLuaVM.runFunction("onAttach", 0);

			test::Sprite s;
			mLuaVM.runFunction("printSprite", 0, s);
			s.draw();
			mLuaVM.runFunction("printSprite", 0, s);
			s.draw();
		}
	}

	void ZeroLuaLayer::onDetach()
	{
		if (mLuaVM)
			mLuaVM.runFunction("onDetach", 0);
	}

	void ZeroLuaLayer::onUpdate(const zr::Time& elapsedTime)
	{
		if (mLuaVM) {
			// int returnedValues = testFunction(4, "Lua test", 3.14);
			mLuaVM.runFunction("onUpdate", 0, elapsedTime);
		}
	}

	void ZeroLuaLayer::onEvent(zr::Event& e)
	{
		if (mLuaVM)
			mLuaVM.runFunction("onEvent", 0, e);
	}

	void ZeroLuaLayer::onImGuiRender()
	{
	}

	int ZeroLuaLayer::testFunction(unsigned a, const char* b, double c)
	{
		if (mLuaVM)
			mLuaVM.runFunction("testFunction", 3, a, b, c);
		return 0;
	}

	void ZeroLuaLayer::drawEditorWindow()

	{
		zr::Window& window = zr::Application::GetWindow();
		ImGui::SetNextWindowSize(ImVec2(.66f * window.getWidth(), .66f * window.getHeight()), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(.66f * window.getWidth(), .66f * window.getHeight()), ImVec2(.8f * window.getWidth(), .8f * window.getHeight()));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("View##edit_view", (bool*)false, ImGuiWindowFlags_NoCollapse);
		{
			auto windowFound = ImGui::FindWindowByName("View##edit_view");
			if (windowFound) {
				ImGuiContext* context = ImGui::GetCurrentContext();
				//mEditViewCanReceiveInput = context && context->HoveredWindow && windowFound->ID == context->HoveredWindow->ID && !windowFound->Collapsed;
			}
			else {
				//mEditViewCanReceiveInput = false;
			}

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 pos0 = ImGui::GetWindowContentRegionMin();
			ImVec2 pos1 = ImGui::GetWindowContentRegionMax();

			ImVec2 avail = ImGui::GetContentRegionAvail();
			//std::cout << "avail = " << avail.x << " " << avail.y << "\n";

			/*if (avail.x != mFramebufferSize.x || avail.y != mFramebufferSize.y) {
				mFramebufferSize.x = avail.x;
				mFramebufferSize.y = avail.y;
				zr::WindowResizeEvent e(mFramebufferSize.x, mFramebufferSize.y);
				mCameraController->onEvent(e);
			}*/

			//ImVec2 size(pos1.x - pos0.x, pos1.y - pos0.y);
			ImVec2 size(pos1.x - pos0.x, pos1.y - pos0.y);

			//ImGui::Text("WindowPosition: (%f, %f)\nPos0: (%f, %f)\nPos1: (%f, %f)", pos.x, pos.y, pos0.x, pos0.y, pos1.x, pos1.y);

			/*ImGui::GetWindowDrawList()->AddImage((void*)(unsigned*)mFramebuffer->getTextureHandle(), pos, ImVec2(pos.x + size.x, pos.y + size.y), { 0.f, 1.f }, { 1.f, 0.f });*/
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}
}
