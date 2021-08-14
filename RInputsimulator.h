#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>




#include "RWait.h"
#include "CV/ImageMatcher.h"


namespace RFramework
{

	class RInputsimulator
	{
		INPUT buffer;
		RWait* rWait = nullptr;

		const int waitTime = 100;
		
		POINT GetScreenSize()
		{
			RECT desktop;
			GetWindowRect(GetDesktopWindow(), &desktop);
			POINT screenSize;
			screenSize.x = desktop.right;
			screenSize.y = desktop.bottom;
			return screenSize;
		}

		void MouseSetup()
		{
			auto screenSize = GetScreenSize();
			
			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = (0 * (0xFFFF / screenSize.x));
			buffer.mi.dy = (0 * (0xFFFF / screenSize.y));
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;
		}

	public:
		void Init(RWait* rWait)
		{
			this->rWait = rWait;
		}
		
		void MoveMouse(int x, int y)
		{
			MouseSetup();
			auto screenSize = GetScreenSize();

			buffer.type = INPUT_MOUSE;
			buffer.mi.mouseData = 0;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;
			buffer.mi.dx = (x * (0xFFFF / screenSize.x));
			buffer.mi.dy = (y * (0xFFFF / screenSize.y));
			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

			SendInput(1, &buffer, sizeof(buffer));
			rWait->Wait(waitTime);
		}

		void MouseClick(int x, int y)
		{
			MouseSetup();
			MoveMouse(x, y);
			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
			SendInput(1, &buffer, sizeof(buffer));
			
			rWait->Wait(waitTime);

			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
			SendInput(1, &buffer, sizeof(buffer));
		}

		void MouseClick(ImageMatchResult match)
		{
			MouseClick(match.x, match.y);
		}

		//void MouseClick(RRectUIElement& rectUIElement)
		//{
		//	auto point = rectUIElement.GetClickPosition();
		//	MouseClick(point.x, point.y);
		//}

		//void PressKey(char mK)
		//{
		//	HKL kbl = GetKeyboardLayout(0);
		//	INPUT ip;
		//	ip.type = INPUT_KEYBOARD;
		//	ip.ki.time = 0;
		//	ip.ki.dwFlags = KEYEVENTF_UNICODE;
		//	if ((int)mK < 65 && (int)mK>90) //for lowercase
		//	{
		//		ip.ki.wScan = 0;
		//		ip.ki.wVk = VkKeyScanEx(mK, kbl);
		//	}
		//	else //for uppercase
		//	{
		//		ip.ki.wScan = mK;
		//		ip.ki.wVk = 0;

		//	}
		//	ip.ki.dwExtraInfo = 0;
		//	SendInput(1, &ip, sizeof(INPUT));
		//}
	};
}
