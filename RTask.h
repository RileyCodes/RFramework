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
	struct RunningState
	{
		bool isRunning = false;
		std::mutex lock;
		std::condition_variable condition;

	};
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
		
	protected:
		RunningState* runningState = nullptr;
		virtual TaskResult _Run() = 0;

	public:
		RTask(string taskName, int cooldown)
		{
			this->taskName = taskName;
			this->rCooldown.Set(cooldown);
		}

		void SetRunningPtr(RunningState& runningState)
		{
			this->runningState = &runningState;
		}

		bool IsRunning()
		{
			return this->runningState->isRunning;
		}
		

		TaskResult Run()
		{
			if (!rCooldown.Try())
				return TaskResult::COOLDOWN;

			return _Run();
		}
	};
}