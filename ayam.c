
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

#undef UNICODE			// use 7-bit ASCII

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


#define DEBUG false		// true: debug msg on,   false: debug msg off


DWORD period;
ARRAY_FLOAT *prices;
ARRAY_FLOAT *sma;
ARRAY_FLOAT *stddev;

FLOAT sd_max;
FLOAT profit_loss;
CHAR buf[1000];
BOOL detect_profit;
FLOAT spread;
FLOAT max_profit;
UINT32 counter;

ORDER order;



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


void calc_sma () {
	DWORD last_index, size, i;
	FLOAT mean, tmp;
	
	size = arrayFloat_size(prices);
	last_index = size - 1;

	if (size > period) {
		tmp = .0f;
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
	float mean, tmp, sd;

	size = arrayFloat_size(prices);
	last_index = size - 1;

	if (size > period) {
		tmp = .0f;
		mean = arrayFloat_get(sma, last_index);
		for (i=last_index; i>(last_index-period); i--) {
			tmp += (float) pow( (arrayFloat_get(prices,i) - mean), 2);
		}
		sd = (float) pow(tmp / period, 5e-1);		/* sqrt(x), x^1/2, x^0.5f, x^5e-1 */
		arrayFloat_add(stddev, sd);

	} else {
		arrayFloat_add(stddev, -1.0f);
	}
}


void ayam_init (DWORD mt_period) {
	period = mt_period;				// 100 tick
	prices = arrayFloat_new();
	sma = arrayFloat_new();
	stddev = arrayFloat_new();

	order.state = false;
	order.type = MARKET_OPEN_FLAT;
	order.open_order = .0f;

	sd_max = .0f;
	profit_loss = .0f;

	spread = .00012f;		// 1.2 pips

	detect_profit = false;

	counter = 0;

	// check for errors
	//if (prices != NULL && sma != NULL && stddev != NULL)
		//MessageBoxA(NULL, "Initialization Complete !", "DEBUG", MB_OK);
	
}



DWORD ayam_start (double tick, ANALYZE type) {
	DWORD size;

	size = arrayFloat_size(prices);
	arrayFloat_add(prices, (float) tick);

	calc_sma();
	calc_stddev();


	if (sd_max < arrayFloat_last(stddev))
		sd_max = arrayFloat_last(stddev);

	if (type == ANALYZE_OPEN)
		return open_market();
	else
		return close_market();
	
}


void ayam_deinit () {
	int pl_pips;
	
	pl_pips = (int) (profit_loss * 1e+4f);
	sprintf(buf, "sd_max: %f\nprofit_loss: %f\nProfit/Loss (pips): %d\n", sd_max, profit_loss, pl_pips);

	if (DEBUG == true)
		MessageBoxA(NULL, buf, "ayam_deinit()", MB_OK);

	sprintf(buf, "counter: %d\n", counter);
	MessageBoxA(NULL, buf, "bounce back: ", MB_OK);
	
	arrayFloat_destroy(prices);
	arrayFloat_destroy(stddev);
	arrayFloat_destroy(sma);
}


MARKET_ACTION open_market () {
	DWORD size;
	char signal[10];
	MARKET_ACTION result = MARKET_OPEN_FLAT;

	size = arrayFloat_size(prices);
	strcpy(signal, "");
	detect_profit = false;
	profit_loss = .0f - spread;
	max_profit = profit_loss;


	if (size > period) {
		if (arrayFloat_last(stddev) > .0002f) {
			if (arrayFloat_last(prices) > arrayFloat_last(sma)) {
				strcpy(signal, "BUY");
				result = MARKET_OPEN_BUY;
			} else if (arrayFloat_last(prices) < arrayFloat_last(sma)) {
				strcpy(signal, "SELL");
				result = MARKET_OPEN_SELL;
			}
		} else {
			strcpy(signal, "FLAT");
			result = MARKET_OPEN_FLAT;
		}
	}

	if ((result == MARKET_OPEN_SELL) || (result == MARKET_OPEN_BUY) && order.state == false) {
		if (DEBUG == true) {
			sprintf(buf, "Price: %f\nSMA: %f\nSTDDEV: %f\nsignal: %s",
				arrayFloat_last(prices), arrayFloat_last(sma), arrayFloat_last(stddev), signal);
			MessageBoxA(NULL, buf, "enter_market()", MB_OK);
		}

		order.state = true;
		order.type = result;
		order.open_order = arrayFloat_last(prices);
		//MessageBoxA(NULL, "asd", "enter_market", MB_OK);
	}


	return result;
}


MARKET_ACTION close_market () {
	MARKET_ACTION result;
	DWORD size;
	float dif = .0f;

	result = MARKET_CLOSE_NO;
	size = arrayFloat_size(prices);

	// calculate profit
	if (order.type == MARKET_OPEN_SELL) 
		profit_loss = order.open_order - arrayFloat_last(prices) - spread;
	else if (order.type == MARKET_OPEN_BUY)
		profit_loss = arrayFloat_last(prices) - order.open_order - spread;

	// Detect Profit
	if (order.type == MARKET_OPEN_SELL && profit_loss > .0f)
		detect_profit = true;
	else if (order.type == MARKET_OPEN_BUY && profit_loss > .0f)
		detect_profit = true;
	
	
	
	if (max_profit < profit_loss)
		max_profit = profit_loss;

	// avoid losses after profit detected
	if (order.type == MARKET_OPEN_SELL && profit_loss <= .0f && detect_profit == true) {
		reset_order();
		result = MARKET_CLOSE_SELL_OK;
		return result;
	} else if (order.type == MARKET_OPEN_BUY && profit_loss <= .0f && detect_profit == true) {
		reset_order();
		result = MARKET_CLOSE_BUY_OK;
		return result;
	}

	// Dynamic Profit / improved trailing stop
	if (size > period && detect_profit == true && order.state == true) {
		// TODO:
		FLOAT tp_checkpoint[13] = {3.f, 5.f, 10.f, 15.f, 20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f, 100.f};
		FLOAT tp_ratio[13] = {.3333f, .6f, .5f, .6f, .65f, .7333f, .775f, .8f, .8f, .8f, .8f, .8f, .8f};
		int i;

		for (i=13; i>=0; --i) {
			if (max_profit >= tp_checkpoint[i] && profit_loss <= (tp_checkpoint[i] * tp_ratio[i])) {
				counter++;

				if (order.type == MARKET_OPEN_SELL)
					result = MARKET_CLOSE_SELL_OK;
				else if (order.type == MARKET_OPEN_BUY)
					result = MARKET_CLOSE_BUY_OK;

				reset_order();
				return result;
			}
		}

		/*if (max_profit >= .0005f && profit_loss <= .0003f) {
			counter++;

			if (order.type == MARKET_OPEN_SELL)
				result = MARKET_CLOSE_SELL_OK;
			else if (order.type == MARKET_OPEN_BUY)
				result = MARKET_CLOSE_BUY_OK;

			reset_order();
			return result;
		}*/
	}


	// Take Profit if stddev < C
	if (size > period && order.state == true) {
		if (order.type == MARKET_OPEN_SELL) {
			if (arrayFloat_last(stddev) < .0002f) {
				reset_order();
				result = MARKET_CLOSE_SELL_OK;
			}
		} else if (order.type == MARKET_OPEN_BUY) {
			if (arrayFloat_last(stddev) < .0002f) {
				reset_order();
				result = MARKET_CLOSE_BUY_OK;
			}
		}
	}
	
	return result;
}

void reset_order () {
	order.state = false;
	order.type = MARKET_OPEN_FLAT;
	order.open_order = .0f;

	detect_profit = false;
}

void ayam_mt4stoploss () {
	reset_order();
}

FLOAT pips2point (FLOAT val_pips) {
	return val_pips / 1e-4f;
}

void COM_test () {
	IUnknown i;
	IUnknownVtbl j;
}

//MessageBoxA(NULL, "asd", "enter_market", MB_OK);