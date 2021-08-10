#pragma once
#include <thread>

#include "RTask.h"
#include "RTaskMgr.h"

#include "Linq/Remove.h"

namespace RFramework
{
	class RLinearTaskMgr : public RTaskMgr
	{
		std::vector<RTask*> tasks;

	public:
		void AddTask(RTask* task)
		{
			tasks.emplace_back(task);
		}

		void RemoveTask(RTask* task)
		{
			tasks < -Remove([&task](RTask* _task) {return _task == task; });
		}

		void Run()
		{
			for (auto&& task : tasks)
			{
				auto result = task->Run();
				if (result == TaskResult::FAILED)
					break;
			}
		}

		void Start()
		{
			std::chrono::milliseconds timespan(5);
			while(1)
			{
				Run();
				std::this_thread::sleep_for(timespan);
			}
		}
	};

}