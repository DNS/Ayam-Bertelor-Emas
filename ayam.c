
//#pragma argsused

#include "ayam.h"
#include "darray.h"


DWORD period;
bool run_once = true;
ARRAY_FLOAT *prices;

int _libmain (unsigned long reason) {
	return 1;
}




void test_test_123 () {
	if (run_once) {
		MessageBox(NULL, "Ayam Bertelor Emas Launched!", "Result", MB_OK);
		run_once = false;
	}
}

void sma (ARRAY_FLOAT *arr, DWORD period) {

}

void stddev (ARRAY_FLOAT *arr, DWORD period) {

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

}



__declspec(dllexport) void __cdecl ayam_deinit () {
	arrayFloat_destroy(prices);
}


