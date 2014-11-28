
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




__declspec(dllexport) void winapi_MessageBoxA (const CHAR *, const CHAR *);
__declspec(dllexport) void winapi_MessageBoxW (const WCHAR *, const WCHAR *);

__declspec(dllexport) void initPriceLog (WCHAR *);
__declspec(dllexport) void addPriceLog (WCHAR *, double, double, double, double, unsigned long);
__declspec(dllexport) void closePriceLog ();



