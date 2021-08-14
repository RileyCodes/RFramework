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
		std::function<bool(int) > action = [](int time)
		{
			Sleep(time);
			return true;
		};
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

