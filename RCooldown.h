#pragma once
#include "RStopwatch.h"

class RCooldown
{
	RStopwatch stopwatch;
	int cooldown = 0;
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
		if(stopwatch.GetElapsedTime() >= cooldown)
		{
			stopwatch.Restart();
			return true;
		}
		return false;
	}
};

