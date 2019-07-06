#include <pch.h>

#include <fmod.hpp>
#include <fmod_errors.h>

namespace lp
{
	class SoundSystem
	{
		public:
			SoundSystem()
			{
				FMOD_RESULT result;

				result = FMOD_System_Create(&mSystem);      // Create the main system object.
				if (result != FMOD_OK)
				{
					printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
					exit(-1);
				}
				
				FMOD_INITFLAGS initFlags = FMOD_INIT_NORMAL;
				result = FMOD_System_Init(mSystem, 32, initFlags, nullptr);
				if (result != FMOD_OK)
				{
					printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
					exit(-1);
				}
			}
			
			void getCPUUsage(std::vector<float>& cpuUsage)
			{
				float dsp;
				float stream;
				float geometry;
				float update;
				float total;
				FMOD_System_GetCPUUsage(mSystem, &cpuUsage[0], &cpuUsage[1], &cpuUsage[2], &cpuUsage[3], &cpuUsage[4]);
			}
			
			void release()
			{
				FMOD_System_Close(mSystem);
				FMOD_System_Release(mSystem);
			}
			
			void printVersion()
			{
				unsigned version;
				FMOD_System_GetVersion(mSystem, &version);
				unsigned minor, major, prodVersion;
				minor = version & 0xFF;
				major = (version >> 8) & 0xFF;
				prodVersion = (version >> 16) & 0xFFFF;
				
				std::cout << "Version: " << std::hex << prodVersion << '.' << major << '.' << minor << '\n';
				std::cout << std::dec;
			}
			
			void update()
			{
				FMOD_System_Update(mSystem);
			}
			
		private:
			FMOD_SYSTEM* mSystem = nullptr;
	};
}
