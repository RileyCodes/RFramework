#include "pch.h"
#include "DllExport.h"
#include "pch.h"

extern int __cdecl FromCaller(char* buffer, int length)
{
	
    return length;
}

extern void __cdecl SetupLogCallback(void(*cb)())
{
	(*cb)();
}

