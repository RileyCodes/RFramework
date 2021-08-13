#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>

#include "RFrameworkException.h"



namespace RFramework
{
	//Framework LogTypes, reserve log id up to 99
	enum class LogType : int
	{
		FatalError,
		LanguageNotFound,
		TaskStarted,
		TaskStopped,
	};

	
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};
	class RLog
	{
		std::ofstream logFile;
	public:
		RLog()
		{
			logFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			
			time_t rawTime;
			char buffer[80] = {0};
			time(&rawTime);
			struct tm timeInfo;

			time_t now = time(0);
			localtime_s(&timeInfo, &now);

			strftime(buffer, sizeof(buffer), "%d-%m-%Y.txt", &timeInfo);
			try
			{
				logFile.open(buffer, std::ios_base::app);
			}
			catch (const std::ofstream::failure& e) {
				std::string errorMsg = "Exception opening file: \n";
				errorMsg += e.what();
				throw RFrameworkException(errorMsg);
			}
		}
		~RLog()
		{
			logFile.close();
		}

		std::string GetLogTime()
		{
			time_t rawTime;
			char buffer[80] = { 0 };
			time(&rawTime);
			struct tm timeInfo;

			time_t now = time(0);
			localtime_s(&timeInfo, &now);

			strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeInfo);
			return buffer;
		}

		
		template <typename T>
		void Add(LogLevel logLevel, T type, std::string msg)
		{
			try
			{
				std::string logMessage = "{" + GetLogTime() + "}" + "[" + std::to_string(static_cast<int>(logLevel)) + "]" +
					"<" + std::to_string(static_cast<int>(type)) + ">:" + msg + "\n";
				logFile << logMessage;
				logFile.flush();
#ifdef _DEBUG
				std::cout << logMessage << std::endl;
#endif
				
				
			}
			catch (const std::ofstream::failure& e) {
				std::string errorMsg = "Exception while write log: \n";
				errorMsg += e.what();
				throw RFrameworkException(errorMsg);
			}
		}

	};


}
