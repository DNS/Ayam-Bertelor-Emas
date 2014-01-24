
//#pragma argsused

#undef UNICODE	// use 7-bit ASCII

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <tchar.h>
#include <stdbool.h>
#include <windows.h>

#include "darray.h"
#include "ayam.h"


#define DEBUG 1


DWORD period;
ARRAY_FLOAT *prices;
ARRAY_FLOAT *sma;
ARRAY_FLOAT *stddev;

float sd_max;
char buf[512];

BOOL APIENTRY DllMain(HMODULE hModule, DWORD nReason, LPVOID Reserved) {
	switch (nReason) {
		case DLL_PROCESS_ATTACH:
			//DisableThreadLibraryCalls(hModule);	//  For optimization
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}




void test_test_123 () {
	MessageBoxA(NULL, "Ayam Bertelor Emas Launched!", "Initialization", MB_OK);
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

void ayam_init (DWORD _period) {
	period = _period;
	prices = arrayFloat_new();
	sma = arrayFloat_new();
	stddev = arrayFloat_new();

	sd_max = 0.0f;

	//if (prices != NULL && sma != NULL && stddev != NULL)
		//test_test_123();


	
}



DWORD ayam_start (double tick) {
	DWORD size;
	size = arrayFloat_size(prices);

	arrayFloat_add(prices, (float) tick);

	calc_sma();
	calc_stddev();

	if (sd_max < arrayFloat_last(stddev) ) {
		sd_max = arrayFloat_last(stddev);
	}


    return enter_market();
}



void ayam_deinit () {
	sprintf(buf, "sd_max: %f\n", sd_max);
	MessageBoxA(NULL, buf, "sd_max", MB_OK);

	arrayFloat_destroy(prices);
	arrayFloat_destroy(stddev);
	arrayFloat_destroy(sma);
}



FORECAST enter_market() {
	DWORD size;
	char signal[256];
	size = arrayFloat_size(prices);
	strcpy(signal, "");

	if (size > period) {
		if (DEBUG == 1) {
			if (arrayFloat_last(stddev) > 0.0002f) {
				if (arrayFloat_last(prices) > arrayFloat_last(sma)) {
					strcpy(signal, "BUY");
                    return BUY;
				} else {
					strcpy(signal, "SELL");
                    return SELL;
                }
			}
			//sprintf(buf, "Price: %f\nSMA: %f\nSTDDEV: %f\nsignal: %s", arrayFloat_last(prices), arrayFloat_last(sma), arrayFloat_last(stddev), signal);

			//MessageBoxA(NULL, buf, "enter_market", MB_OK);
		}
	}
}


//MessageBoxA(NULL, "asd", "enter_market", MB_OK);