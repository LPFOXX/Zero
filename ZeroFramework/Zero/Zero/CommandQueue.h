#pragma once

#include <queue>
#include <functional>

#include "../vendor/sfml/include/SFML/System.hpp"

namespace zr
{
	class CommandQueue
	{
	public:
		CommandQueue() = default;
		virtual ~CommandQueue() = default;
		static void Enqueue(std::function<void()> command);
		static void ExecuteCommands();

	private:
		static std::queue<std::function<void()>> sCommands;
		static sf::Mutex sQueueMutex;
	};
}
