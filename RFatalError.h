#pragma once
#include "pch.h"
#include <string>


#include "RLocalization.h"
#include "Rlog.h"



namespace RFramework
{

	class RFatalError
	{
	public:
		static void Raise(RLog* rLog, RLocalization* rLocal, std::string msg);
	};

}
