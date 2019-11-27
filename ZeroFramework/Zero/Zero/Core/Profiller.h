#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <thread>

#include "Clock.h"

#ifdef ZR_PROFILING
	#define GLUE(a, b) a##b
	#define ZR_PROFILER_BEGIN_SESSION(name, filepath) zr::Profiller::Get().beginSession(name, filepath)
	#define ZR_PROFILER_END_SESSION() zr::Profiller::Get().endSession()
	#define ZR_PROFILER_SCOPE(name) zr::Profiller::Timer GLUE(profilerTimer,__LINE__)(name)
	#define ZR_PROFILER_FUNCTION() ZR_PROFILER_SCOPE(__func__)
#else
	#define ZR_PROFILER_BEGIN_SESSION(name, filepath)
	#define ZR_PROFILER_END_SESSION()
	#define ZR_PROFILER_SCOPE(name)
	#define ZR_PROFILER_FUNCTION()
#endif

namespace zr
{
	class Profiller
	{
	public:
		struct Result
		{
			Result() :
				Name(),
				Start(),
				End(),
				ThreadId(0U)
			{

			}

			Result(const std::string& name, const Time& start, const Time& end, unsigned threadID = 0U) :
				Name(name),
				Start(start),
				End(end),
				ThreadId(threadID)
			{

			}

			std::string Name;
			Time Start, End;
			unsigned ThreadId;
		};

		struct Session
		{
			Session() :
				Name()
			{

			}

			Session(const std::string& name) :
				Name(name)
			{

			}

			std::string Name;
		};

		class Timer
		{
		public:
			Timer(const char* name) :
				mName(name),
				mStartTime(Time::Now())
			{

			}

			virtual ~Timer()
			{
				auto thisThreadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
				Profiller::Get().writeProfile({ mName, mStartTime, Time::Now() , (unsigned)thisThreadId });
			}

		private:
			const char* mName;
			Time mStartTime;
		};

		Profiller() :
			mCurrentSession(nullptr),
			mOutputStream(),
			mProfileCount(0)
		{

		}

		virtual ~Profiller()
		{

		}

		void beginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			mOutputStream.open(filepath);
			mOutputStream << std::fixed;
			writeHeader();
			mCurrentSession = new Session(name);
		}

		void endSession()
		{
			writeFooter();
			mOutputStream.close();
			delete mCurrentSession;
			mProfileCount = 0U;
		}

		void writeProfile(const Result& result)
		{
			if (mProfileCount++ > 0) {
				mOutputStream << ",";
			}

			std::string name = result.Name;
			Time end = result.End;
			std::replace(name.begin(), name.end(), '"', '\'');

			mOutputStream << "{";
			mOutputStream << "\"cat\":\"function\",";
			mOutputStream << "\"dur\":" << (end - result.Start).asMicroseconds() << ',';
			mOutputStream << "\"name\":\"" << name << "\",";
			mOutputStream << "\"ph\":\"X\",";
			mOutputStream << "\"pid\":0,";
			mOutputStream << "\"tid\":" << result.ThreadId << ",";
			mOutputStream << "\"ts\":" << static_cast<unsigned>(result.Start.asMicroseconds());
			mOutputStream << "}";

			mOutputStream.flush();
		}

		void writeHeader()
		{
			mOutputStream << "{\"otherData\": {}, \"traceEvents\":[";
			mOutputStream.flush();
		}

		void writeFooter()
		{
			mOutputStream << "]}";
			mOutputStream.flush();
		}

		static Profiller& Get()
		{
			static Profiller* instance = new Profiller();
			return *instance;
		}

	private:
		Session* mCurrentSession;
		std::ofstream mOutputStream;
		unsigned mProfileCount;
	};
}
