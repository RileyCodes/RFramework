#include "pch.h"
#include "RInputSimulator.h"

bool RFramework::RInputSimulator::Action(std::function<bool()> action, int retryCount, int increaseThreshold)
{
	for (auto i = 0; i < retryCount; i++)
	{
		//if (i > increaseThreshold)
		//	this->IncreaseAdditionalWaitTime();

		if (action())
			return true;
	}
	return false;
}
