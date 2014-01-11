
#ifndef _AYAM_H
#define _AYAM_H

#include "darray.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MARKET_FLAT 0
#define MARKET_BUY 1
#define MARKET_SELL 2


DWORD enter_market ();
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

