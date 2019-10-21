#include <zr_pch.h>

#include "Core/Log.h"
#include "ModelLogger.h"

namespace zr
{
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
		ZR_CORE_TRACE("[ASSIMP] {0}", message);
	}

	void ModelLogger::OnInfo(const char* message)
	{
		ZR_CORE_INFO("[ASSIMP] {0}", message);
	}

	void ModelLogger::OnWarn(const char* message)
	{
		ZR_CORE_WARN("[ASSIMP] {0}", message);
	}

	void ModelLogger::OnError(const char* message)
	{
		ZR_CORE_ERROR("[ASSIMP] {0}", message);
	}
}
