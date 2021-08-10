#pragma once

class RTaskStopException : public std::exception
{
    std::string errMessage = "Task Stopped";
public:

    RTaskStopException(std::string errorMsg);

	RTaskStopException()
	{
        auto rLog = RFramework::RApplication::GetRLog();
        auto rLocal = RFramework::RApplication::GetRLocalization();
        rLog->Add(RFramework::LogLevel::Info, RFramework::LogType::TaskStopped, rLocal->GetA("TASK_STOPPED"));
	}

    const char* what() const throw ()
    {
        return errMessage.c_str();
    }
};
