#include "pch.h"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "RLocalization.h"

using namespace std;
void RFramework::RLocalization::Parse(stringstream& jsonString)
{
	auto jsonArray = nlohmann::json::parse(jsonString);
	for (auto item : jsonArray.items())
	{
		auto languageObject = item.value();
		string languageName = languageObject["Lang"];

		for (auto iteration_proxy_value : languageObject.items())
		{
			string keyName = iteration_proxy_value.key();
			if (keyName == "Lang")
				continue;

			string value = iteration_proxy_value.value();

			translate[languageName][keyName] = value;

			
		}
	}

}
