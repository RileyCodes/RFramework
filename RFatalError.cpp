#include "pch.h"
#include "RFatalError.h"
#include <Windows.h>



namespace RFramework
{
	void RFatalError::Raise(RLog* rLog, RLocalization* rLocal, std::string msg)
	{
		std::wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wmsg = converter.from_bytes(msg);

		if (rLog)
			rLog->Add(LogLevel::Error, LogType::FatalError, msg);

		if (rLocal)
			MessageBoxW(NULL, wmsg.c_str(), rLocal->GetW("FATAL_ERROR").c_str(), MB_OK);
		else
			MessageBoxW(NULL, wmsg.c_str(), L"Fatal Error", MB_OK);




		exit(-1);
	}
}
