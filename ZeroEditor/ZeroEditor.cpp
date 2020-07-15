#include <pch.hpp>

#include "EditorLayer.hpp"
#include "ZeroEditor.hpp"
#include "ZeroLuaLayer.hpp"

#include "Zero/Zero/Core/EntryPoint.h"

#include "Zero/Zero/ImageWriter.h"

namespace lp
{
	ZeroEditor::ZeroEditor() :
		zr::Application("")
	{
		ZR_PROFILER_FUNCTION();
		zr::Application::GetWindow().setIcon("resources/textures/logo.png");
		pushLayer(new EditorLayer);
	}

	ZeroEditor::~ZeroEditor()
	{
		ZR_PROFILER_FUNCTION();
	}
}

zr::Application* zr::createApplication()
{
	return new lp::ZeroEditor;
}
