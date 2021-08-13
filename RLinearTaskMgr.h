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

		RunningState runningState;

		void _Start()
		{
			std::unique_lock <std::mutex> lock(runningState.lock);
			runningState.isRunning = true;
		}
	
	public:
		void AddTask(RTask& task)
		{
			task.SetRunningPtr(runningState);
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
				if (!runningState.isRunning)
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
			_Start();
			
			auto rLog = RFramework::RApplication::GetRLog();
			auto rLocal = RFramework::RApplication::GetRLocalization();
			rLog->Add(RFramework::LogLevel::Info, LogType::TaskStarted, rLocal->GetA("TASK_STARTED"));
			
			std::chrono::milliseconds timespan(5);
			while(1)
			{
				Run();
				std::this_thread::sleep_for(timespan);
			}
		}

		void Stop()
		{
			std::unique_lock <std::mutex> lock(runningState.lock);
			runningState.isRunning = false;
			runningState.condition.notify_all();
		}

		bool IsRunning()
		{
			return runningState.isRunning;
		}
		
	};

}