#pragma once
#include <chrono>

using namespace std::chrono;
class RStopwatch
{
	steady_clock::time_point begin = steady_clock::now();
	bool isStarted = false;

public:
	void Restart()
	{
		begin = steady_clock::now();
	}

	int GetElapsedTime() const
	{
		if (!isStarted)
			return 0;
		
		auto now = steady_clock::now();
		return duration_cast<microseconds>(now - begin).count();
	}
};

