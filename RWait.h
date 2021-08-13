#pragma once
#include <thread>

#include "RStopwatch.h"


namespace RFramework
{
	class RWait
	{
		std::function<bool(int) > action = nullptr;
	public:

		void Init(std::function<bool(int) > action)
		{
			this->action = action;
		}
		
		void Wait(int time)
		{
			action(time);
		}
	};
}

