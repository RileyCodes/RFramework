#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>

#include "CV/ImageMatcher.h"


namespace RFramework
{

	class RInputsimulator
	{
		INPUT buffer;

		std::chrono::milliseconds timespan;
		
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

		RInputsimulator()
		{
			timespan = std::chrono::milliseconds(100);
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
			std::this_thread::sleep_for(timespan);
		}

		void MouseClick(int x, int y)
		{
			MouseSetup();
			MoveMouse(x, y);
			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
			SendInput(1, &buffer, sizeof(buffer));

			std::this_thread::sleep_for(timespan);

			buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
			SendInput(1, &buffer, sizeof(buffer));
		}

		void MouseClick(ImageMatchResult match)
		{
			MouseClick(match.x, match.y);
		}

	};
}
