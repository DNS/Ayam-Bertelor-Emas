
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

//#ifndef _ABE_H
//#define _ABE_H

#include <windows.h>

typedef struct {
	unsigned long size;
	float *values;
} PRICE_LOG;


#ifdef __cplusplus
extern "C" {
#endif



__declspec(dllexport) void winapi_MessageBoxA (const CHAR *, const CHAR *);
__declspec(dllexport) void winapi_MessageBoxW (const WCHAR *, const WCHAR *);

#ifdef __cplusplus
}
#endif


/* _ABE_H */
//#endif

