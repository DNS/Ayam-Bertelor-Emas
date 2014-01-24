
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


#undef UNICODE	// use 7-bit ASCII

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <tchar.h>
#include <stdbool.h>

#include "darray.h"
#include "ayam.h"


#define DEBUG true		// true: debug msg on, false: debug msg off


DWORD period;
ARRAY_FLOAT *prices;
ARRAY_FLOAT *sma;
ARRAY_FLOAT *stddev;

float sd_max;
char buf[500];

BOOL APIENTRY DllMain(HMODULE hModule, DWORD nReason, LPVOID Reserved) {
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


void calc_sma () {
	DWORD last_index, size, i;
	float tick, mean, tmp;

	size = arrayFloat_size(prices);
	last_index = size - 1;

	if (size > period) {
		tmp = 0.0f;
		for (i=last_index; i>(last_index-period); i--) {
			tmp += arrayFloat_get(prices, i);
		}
		mean = (float) tmp / period;
		arrayFloat_add(sma, mean);

	} else {
		arrayFloat_add(sma, -1.0f);
	}
    
}

/* calc_stddev() must be called after calc_sma() */
void calc_stddev () {
	DWORD last_index, size, i;
	float tick, mean, tmp, sd;

	size = arrayFloat_size(prices);
	last_index = size - 1;

	if (size > period) {
		tmp = 0.0f;
		mean = arrayFloat_get(sma, last_index);
		for (i=last_index; i>(last_index-period); i--) {
			tmp += pow( (arrayFloat_get(prices,i) - mean), 2);
		}
		sd = pow(tmp / period, (double) 5e-1);		/* sqrt(x), x^1/2, x^0.5f, x^5e-1 */
		arrayFloat_add(stddev, sd);

	} else {
		arrayFloat_add(stddev, -1.0f);
	}
}

void ayam_init (DWORD mt_period) {
	period = mt_period;
	prices = arrayFloat_new();
	sma = arrayFloat_new();
	stddev = arrayFloat_new();

	sd_max = 0.0f;

	// check for errors
	//if (prices != NULL && sma != NULL && stddev != NULL)
		//MessageBoxA(NULL, "Initialization Complete !", "DEBUG", MB_OK);

}


DWORD ayam_start (double tick) {
	DWORD size;

	size = arrayFloat_size(prices);
	arrayFloat_add(prices, (float) tick);

	calc_sma();
	calc_stddev();

	if (sd_max < arrayFloat_last(stddev))
		sd_max = arrayFloat_last(stddev);

	return enter_market();
}


void ayam_deinit () {
	sprintf(buf, "sd_max: %f\n", sd_max);
	MessageBoxA(NULL, buf, "sd_max", MB_OK);

	arrayFloat_destroy(prices);
	arrayFloat_destroy(stddev);
	arrayFloat_destroy(sma);
}



FORECAST enter_market () {
	DWORD size;
	char signal[256];
	FORECAST result;

	size = arrayFloat_size(prices);
	strcpy(signal, "");

	if (size > period) {
		if (arrayFloat_last(stddev) > 0.0002f) {
			if (arrayFloat_last(prices) > arrayFloat_last(sma)) {
				strcpy(signal, "BUY");
				result = BUY;
			} else {
				strcpy(signal, "SELL");
				result = SELL;
			}
		}
	}
	
	if (DEBUG == true && (result == BUY || result == SELL)) {
		sprintf(buf, "Price: %f\nSMA: %f\nSTDDEV: %f\nsignal: %s", arrayFloat_last(prices), 
			arrayFloat_last(sma), arrayFloat_last(stddev), signal);
		MessageBoxA(NULL, buf, "enter_market()", MB_OK);
	}

	return result;
}


//MessageBoxA(NULL, "asd", "enter_market", MB_OK);