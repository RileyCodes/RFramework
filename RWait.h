#pragma once
#include <thread>

#include "RStopwatch.h"

namespace RFramework
{
	class RWait
	{
		std::function<bool(*) > action = nullptr;
	public:

		void Init(std::function<bool(*) > action)
		{
			this->action = action;
		}
		
		void Wait(int time)
		{
			RStopwatch watch;
			watch.Restart();
			while (1)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				if (watch.GetElapsedTime() > time)
					break;
				if (!action)
					break;
			}
		}
	};
}
