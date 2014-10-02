
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

#pragma comment(linker,"\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6...0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Comctl32.lib")


#define _NO_DEBUG_HEAP
#define _CRT_SECURE_NO_WARNINGS

/* force MSVC to use ANSI/WideChar function, must be before #include <windows.h> */
#undef UNICODE


#include <windows.h>
#include <commctrl.h>
#include <richedit.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <tchar.h>


#include "abe.h"
#include "darray.h"

FILE *fh;
PRICE_LOG plog;
ARRAY_FLOAT *ticks;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD nReason, LPVOID Reserved) {
	switch (nReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);		//  For optimization (for 1 thread only)
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}



void winapi_MessageBoxA (const CHAR *text, const CHAR *caption) {
	MessageBoxA(NULL, text, caption, MB_OK);
}

void winapi_MessageBoxW (const WCHAR *text, const WCHAR *caption) {
	MessageBoxW(NULL, text, caption, MB_OK);
}

void initPriceLog (WCHAR *wpair) {
	WideCharToMultiByte(CP_ACP, 0, wpair, lstrlenW(wpair), plog.pair, 7, NULL, NULL);

	//lstrcpyW(plog.pair, wpair);

	ticks = arrayFloat_new();

	
}

void flushPriceLog () {
	DWORD i;
	HANDLE file_out;
	DWORD bytesWritten;
	CHAR file_name[500];

	lstrcpyA(file_name, "");
	lstrcatA(file_name, plog.pair);
	lstrcatA(file_name, ".txt");

	file_out = CreateFileA(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	plog.tick_length = arrayFloat_size(ticks);
	
	WriteFile(file_out, plog.pair, sizeof(plog.pair), &bytesWritten, NULL);
	WriteFile(file_out, &plog.tick_length, sizeof(DWORD), &bytesWritten, NULL);
	
	for (i=0; i<plog.tick_length; i++) {
		float tick = arrayFloat_get(ticks, i);
		WriteFile(file_out, &tick, sizeof(float), &bytesWritten, NULL);
	}

	CloseHandle(file_out);
	arrayFloat_destroy(ticks);
}

void addPriceLog (double price) {
	float f_price = price;
	arrayFloat_add(ticks, price);
}



void main () {
	initPriceLog(L"EURUSD");

	addPriceLog(1.12345);
	addPriceLog(2.12345);

	flushPriceLog();


	system("pause");
}

//MessageBoxA(NULL, "asd", "enter_market", MB_OK);




