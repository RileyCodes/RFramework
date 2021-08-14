#pragma once
#include "RStopwatch.h"

class RCooldown
{
	RStopwatch stopwatch;
	int cooldown = -1;
public:
	RCooldown() = default;
	RCooldown(int cooldown)
	{
		stopwatch.Restart();
		this->cooldown = cooldown;
	}

	void Set(int cooldown)
	{
		this->cooldown = cooldown;
	}

	bool Try()
	{
		if (cooldown == -1)//If the cooldown is not enabled, we will not restart the cooldown.
			return true;
		
		if(stopwatch.GetElapsedTime() >= cooldown)
		{
			stopwatch.Restart();
			return true;
		}
		return false;
	}
};

