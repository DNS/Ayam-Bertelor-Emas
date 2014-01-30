
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

typedef struct _ORDER {
	FORECAST type;
	bool state;
	float open_order;
} ORDER;

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

