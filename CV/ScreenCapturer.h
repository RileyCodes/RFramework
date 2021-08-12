#pragma once
#include <windows.h>
#include "ImageCapturer.h"
class ScreenCapturer : ImageCapturer
{
public:
	Mat Capture() override;
	HWND hwnd = nullptr;
	HDC hwindowDC = nullptr;
	HDC hwindowCompatibleDC = nullptr;
	HBITMAP hbwindow = nullptr
	;
	int height = -1;
	int width = -1;
	ScreenCapturer()
	{
		hwindowDC = GetDC(hwnd);	
		hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);
		//TODO: DPI SCALE?
		//TODO: USER CHANGED RESOLUTION AFTER?
		width = desktop.right;
		height = desktop.bottom;
		
		SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
		hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	}

	~ScreenCapturer()
	{
		DeleteObject(hbwindow);
		DeleteDC(hwindowCompatibleDC);
		ReleaseDC(hwnd, hwindowDC);
	}
};

