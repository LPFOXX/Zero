#pragma once

#include "imgui.h"
#include "glm/glm.hpp"

namespace zr
{
	enum class ImguiWidgetType
	{
		NoType,
		Vec3,
		Vec4
	};

	struct ImGuiVec2Component
	{
		glm::vec2 Data;
		void Draw(const char* label)
		{
			ImGui::DragFloat2(label, &Data[0]);
		}
	};

	struct ImGuiVec3Component
	{
		glm::vec3 Data;
		void Draw(const char* label)
		{
			ImGui::DragFloat3(label, &Data[0]);
		}
	};

	struct ImGuiVec4Component
	{
		glm::vec4 Data;
		void Draw(const char* label)
		{
			ImGui::DragFloat4(label, &Data[0]);
		}
	};

	/*struct ImGuiWidgetComponent
	{
		ImGuiWidgetComponent() :
			Type(ImguiWidgetType::NoType),
			Data{0.f, 0.f, 0.f, 0.f}
		{

		}

		ImGuiWidgetComponent(float* data) :
			Type(ImguiWidgetType::NoType),
			Data{ 0.f, 0.f, 0.f, 0.f }
		{

		}

		ImGuiWidgetComponent(const ImGuiWidgetComponent&) = default;

		ImguiWidgetType Type;
		float Data[4];
	};*/

	/*void drawWidget(const char* widgetLabel, ImGuiWidgetComponent& widget)
	{
		switch (widget.Type) {
			case ImguiWidgetType::Vec3: ImGui::DragFloat3(widgetLabel, widget.)
			case ImguiWidgetType::Vec4:
			case ImguiWidgetType::NoType:
			default:
			break;
		}
	}*/
}
