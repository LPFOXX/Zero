#include <zr_pch.h>

#include "CommandQueue.h"

namespace zr
{
	std::queue<std::function<void()>> CommandQueue::sCommands;
	sf::Mutex CommandQueue::sQueueMutex;

	void CommandQueue::Enqueue(std::function<void()>& command)
	{
		sf::Lock l(sQueueMutex);
		sCommands.push(command);
	}

	void CommandQueue::ExecuteCommands()
	{
		sf::Lock l(sQueueMutex);
		while (!sCommands.empty()) {
			sCommands.front()();
			sCommands.pop();
		}
	}
}
