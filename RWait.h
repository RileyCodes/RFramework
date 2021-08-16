#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#pragma once
#include <thread>
#include "RStopwatch.h"
#include <functional>
#include <Windows.h>
namespace RFramework
{
	class RWait
	{
		int additionalWaitTime = 0;
		std::function<bool(int) > action = [](int time)
		{
			Sleep(time);
			return true;
		};
	public:
		void IncreaseAdditionalWaitTime()
		{
			additionalWaitTime = additionalWaitTime >= 500 ? 500 : additionalWaitTime + 20;
		}
		void DecreaseAdditionalWaitTime()
		{
			additionalWaitTime = additionalWaitTime <= 0 ? 0 : additionalWaitTime - 20;
		}
			
		void Init(std::function<bool(int) > action)
		{
			this->action = action;
		}
		
		void Wait(int time)
		{
			if (time <= 0)
				return;

			time += additionalWaitTime;
			action(time);
		}
	};
}

