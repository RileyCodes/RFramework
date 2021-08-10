#pragma once
#include <chrono>
#include "RCooldown.h"

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
	protected:
		virtual TaskResult _Run() = 0;

	public:
		RTask(string taskName, int cooldown)
		{
			this->taskName = taskName;
			this->rCooldown.Set(cooldown);
		}

		TaskResult Run()
		{
			if (!rCooldown.Try())
				return TaskResult::COOLDOWN;

			return _Run();
		}
	};
}