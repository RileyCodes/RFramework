#pragma once
#define  DllExport   __declspec( dllexport )

extern "C"  DllExport int FromCaller(char* buffer, int length);
extern "C"  DllExport void SetupLogCallback(void(*cb)());
extern "C"  DllExport wchar_t* SetupGetBufferPtr(wchar_t* (*mallocBuffer)(int));


inline wchar_t* (*CreateBuffer)(int) = nullptr;

extern void __cdecl SetupLogCallback(void(*cb)())
{
	(*cb)();
}

extern wchar_t* __cdecl SetupGetBufferPtr(wchar_t*(*mallocBuffer)(int))
{
	CreateBuffer = mallocBuffer;
}

