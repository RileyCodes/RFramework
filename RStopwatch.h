#pragma once
#include <chrono>

using namespace std::chrono;
class RStopwatch
{
	steady_clock::time_point begin = std::chrono::steady_clock::now();

public:
	void Restart()
	{
		begin = std::chrono::steady_clock::now();
	}

	int GetElapsedTime() const
	{
		auto now = std::chrono::steady_clock::now();
		return duration_cast<std::chrono::microseconds>(now - begin).count();
	}
};

