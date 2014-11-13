
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
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
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

FILE *file_out;


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
	CHAR file_name[256];
	CHAR pair[16];


	WideCharToMultiByte(CP_ACP, 0, wpair, lstrlenW(wpair), pair, 7, NULL, NULL);

	sprintf(file_name, "D:\\%s-D1-2013.xml", pair);

	file_out = fopen(file_name, "wb");
	fprintf(file_out, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n");
	fprintf(file_out, "<forex pair=\"EURUSD\" period=\"D1\" datetime=\"2013\">\n");

}

void closePriceLog () {
	fprintf(file_out, "</forex>");
	fclose(file_out);
}

void addPriceLog (double price) {
	fprintf(file_out, "<price>%.5f</price>", price);
}


//MessageBoxA(NULL, "asd", "enter_market", MB_OK);


void main () {
	initPriceLog(L"EURUSD");

	addPriceLog(1.12345);
	addPriceLog(2.12345);

	flushPriceLog();


	//system("pause");
}





/*

<?xml version="1.0" encoding="utf-8"?>
*/


