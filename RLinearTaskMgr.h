#pragma once
#include <thread>

#include "RTask.h"
#include "RTaskMgr.h"
#include "RTaskStopException.h"

#include "Linq/Remove.h"

namespace RFramework
{
	class RLinearTaskMgr : public RTaskMgr
	{
		std::vector<RTask*> tasks;
		bool isRunning = false;

	public:
		void AddTask(RTask& task)
		{
			tasks.emplace_back(&task);
		}

		void RemoveTask(RTask* task)
		{
			tasks < -Remove([&task](RTask* _task) {return _task == task; });
		}

		void Run()
		{
			for (auto&& task : tasks)
			{
				if (!isRunning)
				{
					throw RTaskStopException();
				}
				auto result = task->Run();
				if (result == TaskResult::FAILED)
					break;
			}
		}

		void Start()
		{
			auto rLog = RFramework::RApplication::GetRLog();
			auto rLocal = RFramework::RApplication::GetRLocalization();
			rLog->Add(RFramework::LogLevel::Info, LogType::TaskStarted, rLocal->GetA("TASK_STOPPED"));
			isRunning = true;
			std::chrono::milliseconds timespan(5);
			try
			{
				while(1)
				{
					Run();
					std::this_thread::sleep_for(timespan);
				}
			}
			catch (RTaskStopException e) {};
		}

		void Stop()
		{
			isRunning = false;
		}

		bool IsRunning()
		{
			return isRunning;
		}
		
	};

}