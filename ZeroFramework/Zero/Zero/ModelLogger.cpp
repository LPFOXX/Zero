#include <zr_pch.h>

#include "Core/Log.h"
#include "ModelLogger.h"

#include "ImGui/ImGuiConsole.h"

namespace zr
{
	sf::Mutex ModelLogger::sLogMutex;

	ModelLogger::ModelLogger(Assimp::Logger::LogSeverity logSeverity)
	{
		setLogSeverity(logSeverity);
	}

	ModelLogger::~ModelLogger()
	{
	}

	bool ModelLogger::attachStream(Assimp::LogStream* pStream, unsigned int severity)
	{
		return false;
	}

	bool ModelLogger::detatchStream(Assimp::LogStream* pStream, unsigned int severity)
	{
		return false;
	}

	void ModelLogger::OnDebug(const char* message)
	{
		sf::Lock l(sLogMutex);
		ZR_IMGUI_LOG(ConsoleItem::Trace, "[ASSIMP] %s", message);
	}

	void ModelLogger::OnInfo(const char* message)
	{
		sf::Lock l(sLogMutex);
		ZR_IMGUI_LOG(ConsoleItem::Info, "[ASSIMP] %s", message);
	}

	void ModelLogger::OnWarn(const char* message)
	{
		sf::Lock l(sLogMutex);
		ZR_IMGUI_LOG(ConsoleItem::Warn, "[ASSIMP] %s", message);
	}

	void ModelLogger::OnError(const char* message)
	{
		sf::Lock l(sLogMutex);
		ZR_CORE_ERROR("[ASSIMP] {0}", message);
		ZR_IMGUI_LOG(ConsoleItem::Error, "[ASSIMP] %s", message);
	}
}
