#pragma once
#include <codecvt>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <locale>
#include <sstream>
#include "nlohmann/json.hpp"


#include "RFrameworkException.h"
#include "../ArkBot/LogType.h"


namespace RFramework
{
	class RLocalization
	{
		RLog* rLog = nullptr;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translate;
		std::string userLang = "zh-cn";
		std::ifstream localizationFile;

		void Parse(std::stringstream& jsonString);

	public:
		RLocalization(RLog* rLog)
		{
			this->rLog = rLog;
			localizationFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				localizationFile.open("Localization.json");
				std::stringstream jsonString;
				jsonString << localizationFile.rdbuf();

				Parse(jsonString);

			}
			catch (const std::ifstream::failure& e) {
				std::string errorMsg = "Exception while opening Localization File: \n";
				errorMsg += e.what();
				throw RFrameworkException(errorMsg);
			}
		}
		void SetUserLanguage(std::string userLang)
		{
			this->userLang = userLang;
		}

		std::string GetA(std::string msgType)
		{
			if(translate.count(userLang) == 0)
			{
				rLog->Add(LogLevel::Warning, LogType::LanguageNotFound, "Unable to Find Language:" + userLang);
				return msgType;
			}

			if (translate[userLang].count(msgType) == 0)
			{
				rLog->Add(LogLevel::Warning, LogType::LanguageNotFound, "Unable to Find MsgType:" + msgType + " Language:" + userLang);
				return msgType;
			}
			
			return translate[userLang][msgType];
		}

		std::wstring GetW(std::string msgType)
		{
			auto msgA = GetA(msgType);
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(msgA);
		}
	};
}