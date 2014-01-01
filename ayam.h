
#ifndef _AYAM_H
#define _AYAM_H

#include "darray.h"



#define MARKET_FLAT 0
#define MARKET_BUY 1
#define MARKET_SELL 2


DWORD enter_market ();
void test_test_123 ();
void calc_sma ();
void calc_stddev ();

void ayam_init (DWORD _period);
DWORD ayam_start (double tick);
void ayam_deinit ();



#endif

