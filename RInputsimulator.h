#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>



#include "RRectUIElement.h"
#include "RTask.h"
#include "RWait.h"
#include "CV/ImageMatcher.h"


namespace RFramework
{

	class RInputSimulator
	{
		INPUT buffer;
		RWait* rWait = nullptr;
		RWait defaultRWait;

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
		void IncreaseAdditionalWaitTime()
		{
			GetRWait()->IncreaseAdditionalWaitTime();
		}

		void DecreaseAdditionalWaitTime()
		{
			GetRWait()->DecreaseAdditionalWaitTime();
		}

		RWait* GetRWait()
		{
			if (rWait == nullptr)
				return &defaultRWait;
			return rWait;
		}

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
			GetRWait()->Wait(waitTime);
		}

		void MouseClick(int x, int y)
		{
			MouseSetup();
			MoveMouse(x, y);
			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
			SendInput(1, &buffer, sizeof(buffer));
			
			GetRWait()->Wait(waitTime);

			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
			SendInput(1, &buffer, sizeof(buffer));
		}

		void MouseClick(ImageMatchResult match)
		{
			MouseClick(match.x, match.y);
		}

		void MouseClick(const RRectUIElement& rectUIElement)
		{
			auto point = const_cast<RRectUIElement&>(rectUIElement).GetClickPosition();
			MouseClick(point.x, point.y);
		}

		void KeyPress(char mK)
		{
			HKL kbl = GetKeyboardLayout(0);
			INPUT ip;
			ip.type = INPUT_KEYBOARD;
			ip.ki.time = 0;
			ip.ki.dwFlags = KEYEVENTF_UNICODE;
			if (static_cast<int>(mK) < 65 && static_cast<int>(mK)>90) //for lowercase
			{
				ip.ki.wScan = 0;
				ip.ki.wVk = VkKeyScanEx(mK, kbl);
			}
			else //for uppercase
			{
				ip.ki.wScan = mK;
				ip.ki.wVk = 0;

			}
			ip.ki.dwExtraInfo = 0;
			SendInput(1, &ip, sizeof(INPUT));
		}
		void KeyDown(WORD keyCode)
		{
			INPUT Event = { 0 };
			Event.type = INPUT_KEYBOARD;
			Event.ki.dwFlags = KEYEVENTF_SCANCODE;
			Event.ki.wScan = MapVirtualKey(keyCode, 0);
			SendInput(1, &Event, sizeof(Event));
		}

		void KeyUp(WORD keyCode)
		{
			INPUT Event = { 0 };
			Event.type = INPUT_KEYBOARD;
			Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Event.ki.wScan = MapVirtualKey(keyCode, 0);
			SendInput(1, &Event, sizeof(Event));
		}

		void ComboKey(WORD key1,WORD key2)
		{
			KeyDown(key1);
			GetRWait()->Wait(50);
			KeyDown(key2);
			GetRWait()->Wait(50);
			KeyUp(key2);
			GetRWait()->Wait(50);
			KeyUp(key1);
			GetRWait()->Wait(50);
		}

		bool Action(std::function<bool()> action, int retryCount = 0, int increaseThreshold = 2);
	};
}
