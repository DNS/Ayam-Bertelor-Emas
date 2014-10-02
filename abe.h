
/*
Project : Ayam Bertelor Emas
Status  : Complete
Author  : Daniel Sirait <dns@cpan.org>
Copyright   : Copyright (c) 2013 - 2014, Daniel Sirait
License : Proprietary
Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#pragma once

#include <windows.h>



typedef struct {
	CHAR pair[7];		/* eg: EURUSD, GBPUSD, USDIDR + NULL terminated char */
	DWORD tick_length;
	float *ticks;
} PRICE_LOG;



__declspec(dllexport) void winapi_MessageBoxA (const CHAR *, const CHAR *);
__declspec(dllexport) void winapi_MessageBoxW (const WCHAR *, const WCHAR *);

__declspec(dllexport) void initPriceLog (WCHAR *pair);
__declspec(dllexport) void addPriceLog (double);
__declspec(dllexport) void flushPriceLog ();



