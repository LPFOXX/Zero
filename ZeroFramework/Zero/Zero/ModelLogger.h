#pragma once

#include <assimp/Logger.hpp>
#include "../sfml/include/SFML/System.hpp"

namespace zr
{
	class Logger
	{
	public:
		Logger() = default;
		virtual ~Logger() = default;
	};

	class ModelLogger : public Assimp::Logger, public Logger
	{
	public:
		ModelLogger(Assimp::Logger::LogSeverity logSeverity);
		virtual ~ModelLogger();

		virtual bool attachStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;
		virtual bool detatchStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;

	protected:
		virtual void OnDebug(const char* message) override;
		virtual void OnInfo(const char* message) override;
		virtual void OnWarn(const char* message) override;
		virtual void OnError(const char* message) override;

	private:
		static sf::Mutex sLogMutex;
	};
}
