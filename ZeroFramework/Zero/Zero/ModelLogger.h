#pragma once

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
		ModelLogger() = default;
		virtual ~ModelLogger();

		virtual bool attachStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;
		virtual bool detatchStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;

	protected:
		virtual void OnDebug(const char* message) override;
		virtual void OnInfo(const char* message) override;
		virtual void OnWarn(const char* message) override;
		virtual void OnError(const char* message) override;
	};
}
