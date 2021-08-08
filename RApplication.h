
#pragma once
#include "RLocalization.h"
#include "../ArkBot/LogType.h"

namespace RFramework
{
	
	class RApplication
	{
		static RLog* rLog;
		static RLocalization* rLocal;

	public:

		static void Initialization();
		static void Dispose();

		static auto GetRLog() -> RLog*;
		static auto GetRLocalization() -> RLocalization*;
	};

}
