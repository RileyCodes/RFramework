#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#pragma once
#include <exception>
#include <string>

#include "../RApplication.h"

using std::string;
class FileCapturerException : public std::exception
{
    RFramework::RLog* rLog = RFramework::RApplication::GetRLog();
    RFramework::RLocalization* rLocal = RFramework::RApplication::GetRLocalization();
	
    string msg = "Failed to read image from: ";
public:
    FileCapturerException(string filePath)
    {
        msg += filePath;
	    rLog->Add(RFramework::LogLevel::Warning, LogType::FileCapturerException,rLocal->GetA("FAILED_READ_IMAGE_FROM_FILE") + msg);
    }

    const char* what() const throw ()
    {
        return msg.c_str();
    }
};

