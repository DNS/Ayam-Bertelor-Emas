
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
//#define UNICODE
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
	CHAR pair[16] = {0};

	//winapi_MessageBoxA("test", "test");
	WideCharToMultiByte(CP_ACP, 0, wpair, lstrlenW(wpair), pair, 7, NULL, NULL);

	sprintf(file_name, "D:\\%s.xml", pair);

	file_out = fopen(file_name, "wb");
	fprintf(file_out, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\r\n");
	fprintf(file_out, "<forex pair=\"%s\" period=\"H1\" price=\"Open\">\r\n", pair);

}

void closePriceLog () {
	fprintf(file_out, "</forex>\r\n");
	fclose(file_out);
}

void addPriceLog (WCHAR *wdatetime, double price) {
	CHAR date[32] = {0};
	int len = lstrlenW(wdatetime);
	//yyyy.mm.dd
	//MessageBoxW(NULL, wdatetime, L"wdatetime", MB_OK);
	WideCharToMultiByte(CP_ACP, 0, wdatetime, len, date, 16, NULL, NULL);
	date[len] = '\0';

	fprintf(file_out, "\t<price date=\"%s\">%f</price>\r\n", date, price);
}


//MessageBoxA(NULL, "asd", "enter_market", MB_OK);


//void main () {
//	WCHAR *t = L"abc";
//	CHAR s[32];
//
//	//WideCharToMultiByte(CP_ACP, 0, t, lstrlenW(t), s, lstrlenW(t)/2, NULL, NULL);
//
//	printf("%d, %d\n", wcslen(t), lstrlenW(t));
//
//	system("pause");
//}





/*

<?xml version="1.0" encoding="utf-8"?>
*/


