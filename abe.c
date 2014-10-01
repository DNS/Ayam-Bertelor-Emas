
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
#include "abe.h"


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

void writePrice (double price) {
	FILE *fh;
	float val;

	val = (float) price;
	

	fh = fopen("test.txt", "wb");

	fwrite(&val, sizeof(float), 1, fh);


	fclose(fh);
}

void main () {
	writePrice(1.0f);
	puts("123");

	system("pause");
}

//MessageBoxA(NULL, "asd", "enter_market", MB_OK);




