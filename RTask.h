#pragma once
#include <chrono>
#include <mutex>
#include <thread>

#include "RCooldown.h"
#include "RTaskStopException.h"
#include <windows.h>

using std::string;
namespace RFramework
{
	enum class TaskResult
	{
		FAILED,
		COMPLETE,
		COOLDOWN
	};

	class RTask
	{
		string taskName;
		RCooldown rCooldown;
		bool* isRunning = nullptr;
	protected:
		virtual TaskResult _Run() = 0;

	public:
		RTask(string taskName, int cooldown)
		{
			this->taskName = taskName;
			this->rCooldown.Set(cooldown);
		}

		void SetRunningPtr(bool& isRunning)
		{
			this->isRunning = &isRunning;
		}

		bool IsRunning()
		{
			return *this->isRunning;
		}
		

		TaskResult Run()
		{
			if (!rCooldown.Try())
				return TaskResult::COOLDOWN;

			return _Run();
		}
	};
}