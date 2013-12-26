
//#pragma argsused

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tchar.h>
#include <stdbool.h>
#include <windows.h>

#include "ayam.h"
#include "darray.h"


DWORD period;
ARRAY_FLOAT *prices;
ARRAY_FLOAT *sma;
ARRAY_FLOAT *stddev;

int _libmain (unsigned long reason) {
	return 1;
}




void test_test_123 () {
	MessageBox(NULL, "Ayam Bertelor Emas Launched!", "Result", MB_OK);
}

void calc_sma () {
	DWORD last_index, size, i;
	float tick, mean, tmp;
    size = arrayFloat_size(prices);
	last_index = size - 1;
	if (size > period) {
    	tmp = 0.0f;
		for (i=last_index; i<(last_index-period); i++) {
            tmp += arrayFloat_get(prices, i);

		}
        mean = tmp / period;
        arrayFloat_add(sma, mean);

	} else {
		arrayFloat_add(sma, -1);
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
        for (i=last_index; i<(last_index-period); i++) {
             tmp += pow( arrayFloat_get(prices, i) - mean, 2);
        }
        sd = pow(tmp, (double) 5e-1);	/* sqrt(x), x^1/2, x^0.5, x^5e-1 */
        arrayFloat_add(stddev, sd);

    } else {
        arrayFloat_add(stddev, -1);
    }
}

__declspec(dllexport) void __cdecl ayam_init (DWORD _period) {
	test_test_123();
	period = _period;
	prices = arrayFloat_new();
}



__declspec(dllexport) void __cdecl ayam_start (double tick) {
	char buf[256];

	arrayFloat_add(prices, (float) tick);
	
	sprintf(buf, "Price: %f\nOk!", arrayFloat_last(prices));

	MessageBox(NULL, buf, "Result", MB_OK);

	calc_sma();
	calc_stddev();
	
}



__declspec(dllexport) void __cdecl ayam_deinit () {
	arrayFloat_destroy(prices);
    arrayFloat_destroy(stddev);
    arrayFloat_destroy(sma);
}


