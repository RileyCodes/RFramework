#include "pch.h"
#include "RApplication.h"

#include "RFatalError.h"


namespace RFramework
{
	RLog* RApplication::rLog = nullptr;
	RLocalization* RApplication::rLocal = nullptr;

	void RApplication::Initialization()
	{
		try
		{
			rLog = new RLog();
			rLocal = new RLocalization(rLog);
		}
		catch (RFrameworkException e)
		{
			std::string msg = "Error occurred during initialization on RFramework:\n";
			msg = msg + e.what();
			RFatalError::Raise(nullptr, nullptr, msg);
		}
	}

	void RApplication::Dispose()
	{
		if (!rLog)
		{
			delete rLog;
			rLog = nullptr;
		}

		if (!rLocal)
		{
			delete rLocal;
			rLocal = nullptr;
		}
	}

	auto RApplication::GetRLog() -> RLog*
	{
		if (rLog == nullptr)
			RFatalError::Raise(nullptr, nullptr, "RLog is not initialized!");
		return rLog;
	}

	auto RApplication::GetRLocalization() -> RLocalization*
	{
		if (rLocal == nullptr)
			RFatalError::Raise(nullptr, nullptr, "RLocalization is not initialized!");
		return rLocal;
	}

}
