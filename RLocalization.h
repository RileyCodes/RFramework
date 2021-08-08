#pragma once
#include <codecvt>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <locale>
#include <sstream>
#include "nlohmann/json.hpp"


#include "RFrameworkException.h"

using namespace std;

namespace RFramework
{
	class RLocalization
	{
		unordered_map<string, unordered_map<string, string>> translate;
		string userLang = "zh-cn";
		ifstream localizationFile;

		void Parse(stringstream& jsonString);

	public:
		RLocalization()
		{
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




		void SetUserLanguage(string userLang)
		{
			this->userLang = userLang;
		}

		string GetA(string msgType)
		{
			if(translate.count(userLang) == 0)
			{
				
				return "NO_STRING";
			}
			
			return translate[userLang][msgType];
		}

		wstring GetW(string msgType)
		{
			auto msgA = GetA(msgType);
			wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(msgA);
		}
	};
}