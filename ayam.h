
#ifndef _AYAM_H
#define _AYAM_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	FLAT = 0,
	BUY = 1,
	SELL = 2
} FORECAST;

FORECAST enter_market();
void test_test_123 ();
void calc_sma ();
void calc_stddev ();

__declspec(dllexport) void __cdecl ayam_init (DWORD _period);
__declspec(dllexport) DWORD __cdecl ayam_start (double tick);
__declspec(dllexport) void __cdecl ayam_deinit ();


#ifdef __cplusplus
}
#endif


#endif

