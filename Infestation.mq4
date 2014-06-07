//+------------------------------------------------------------------+
//|                                                  Infestation.mq4 |
//|                                    Copyright 2014, Daniel Sirait |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Sirait"
#property link"http://www.siraitx.com/"
#property version "1.00"
#property description "Infestation v0.0.1-alpha"
#property strict

#define MAGICNUMBER  12052014

#import "Ayam Bertelor Emas.dll"
void winapi_MessageBoxW (string, string);
#import


int SL = (14 - 1.2) * 10;	// 13, 30, 40 pips SL ?
int TP = (1000) * 10;	// 1000 pips
int TrailingStop = 20;
double lots = 0.01;
double spread = 0.0;
int leverage = 1;	// set initial leverage value
string buf;
int ticket = -1;
int period = PERIOD_H1;

bool is_trade = false;
bool wait_trade = true;
bool first_run = true;
bool detect_profit = false;
bool skip_bar = false;
int count_bar = 0;

double k = 2.0;

double tenkan_sen[90];
double kijun_sen[90];
double senkou_span_a[90];
double senkou_span_b[90];
double chinkou_span[90];
double sd[90];
double ma[90];


enum FORECAST {
	FC_FLAT = 0,
	FC_SHORT = 1,
	FC_LONG = 2,
	FC_SHORT_FORCE = 3,
	FC_LONG_FORCE = 4
} forecast;



void OnTimer () {

}


double OnTester () {
	double ret = 0.0;
	
	//winapi_MessageBoxW("OnTester()\n Back testing end", "DEBUG");
	return(ret);
}


void OnChartEvent (const int id, const long &lparam, const double &dparam, const string &sparam) {

}





//




int OnInit () {
	if (!IsTradeAllowed()) {
		winapi_MessageBoxW("Trade not allowed!, please consult your broker.", "Error!");
	}
	
	
	getMarketData();
	
	return(INIT_SUCCEEDED);
}


void OnDeinit (const int reason) {
	
}


void OnTick () {
	//getMarketData();
	if(Bars<90) return;
	
	for (int i=0; i<90; i++) {
		// shift 1
		//tenkan_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_TENKANSEN, i);
		kijun_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_KIJUNSEN, i);
		senkou_span_a[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANA, i);
		senkou_span_b[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANB, i);
		//chinkou_span[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_CHINKOUSPAN, i);
		//sd[i] = iStdDev(Symbol(), period, 20, 0, MODE_SMA, PRICE_CLOSE, i);
		ma[i] = iMA(Symbol(), period, 52, 0, MODE_SMA, PRICE_CLOSE, i);
	}
	
	//winapi_MessageBoxW("tick", "DEBUG");
	analyze_market();
	
	// hit SL/TP
	if (is_trade == true && OrdersTotal() == 0) {
		is_trade = false;
		ticket = -1;
		wait_trade = true;
		detect_profit = false;
		skip_bar = true;
		count_bar = 0;
		/*if (forecast == FC_SHORT) {
			forecast = FC_SHORT_FORCE;
		} else if (forecast == FC_LONG) {
			forecast = FC_LONG_FORCE;
		}*/
	}
	
	
	if (Volume[0] == 1 && skip_bar == true) {
		//winapi_MessageBoxW("skip_bar", "DEBUG");
		if (count_bar >= 0) {		// count_bar >= 0 // disable skip_bar
			skip_bar = false;
			count_bar = 0;
		} else
			count_bar++;
	}
	
	if (OrdersTotal() == 0 
		&& skip_bar == false
		) {
		
		// entry: BUY
		if (
			(forecast == FC_LONG && Ask > senkou_span_a[0] && Ask > senkou_span_b[0]) ||
			(forecast == FC_LONG_FORCE && Ask > senkou_span_a[0]) ||
			(forecast == FC_LONG_FORCE && Ask > senkou_span_b[0])
			
			//Ask > kijun_sen[0]
			) {
			
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			is_trade = true;
		
		// entry: SELL
		} else if (
			(forecast == FC_SHORT && Bid < senkou_span_a[0] && Bid < senkou_span_b[0]) || 
			(forecast == FC_SHORT_FORCE && Bid < senkou_span_a[0]) ||
			(forecast == FC_SHORT_FORCE && Bid < senkou_span_b[0])
			//Bid < kijun_sen[0]
			) {
			
			ticket = OrderSend(Symbol(), OP_SELL, lots, Bid, 3, Ask+SL*Point, Ask-TP*Point, NULL, MAGICNUMBER, 0, Red);
			is_trade = true;
		}
	
	
	// exit order
	} else if (OrdersTotal() != 0) {
	
	
		// Detect Profit
		if (Bid > OrderOpenPrice() && OrderType() == OP_BUY) 
			detect_profit = true;
		else if (Ask < OrderOpenPrice() && OrderType() == OP_SELL)
			detect_profit = true;
		else
			detect_profit = false;
			
		if (Volume[0] > 100 && detect_profit == false) return;
		
		double pl = 0.0;
		if (OrderType() == OP_BUY) pl = Bid - OrderOpenPrice();
		if (OrderType() == OP_SELL) pl = OrderOpenPrice() - Ask;
		
		OrderSelect(ticket, SELECT_BY_TICKET, MODE_TRADES);
		
		if (OrderType() == OP_BUY && detect_profit && pl > 0.0015) {
			if (pl > 0.0050 && Bid < ma[0]) {
				OrderClose(ticket, OrderLots(), Bid, 0, White);
				ticket = -1;
				skip_bar = true;
				is_trade = false;
				count_bar = 0;
			} else if (pl < 0.0050 && Bid < kijun_sen[0]) {
				OrderClose(ticket, OrderLots(), Bid, 0, White);
				ticket = -1;
				skip_bar = true;
				is_trade = false;
				count_bar = 0;
			}
			
			
		
		} else if (OrderType() == OP_SELL && detect_profit && pl > 0.0015) {
			if (pl > 0.0050 && Ask > ma[0]) {
				OrderClose(ticket, OrderLots(), Ask, 0, White);
				ticket = -1;
				skip_bar = true;
				is_trade = false;
				count_bar = 0;
			} else if (pl < 0.0050 && Ask > kijun_sen[0]) {
				OrderClose(ticket, OrderLots(), Ask, 0, White);
				ticket = -1;
				skip_bar = true;
				is_trade = false;
				count_bar = 0;
			}
			
		}
		
	
	}
	
	
	
}


void getMarketData () {
	spread = MarketInfo(Symbol(), MODE_SPREAD) * 1e-5;
	leverage = AccountLeverage();
	Comment("Spread ", spread, "\nLeverage 1:", leverage);
}





void analyze_market () {
	int down_trend = 0, up_trend = 0, flat_trend = 0;	// trend_strength
	int total_bar = 84;					// 84 = 12 * 7;
	
	
	//for (int i=1; i<total_bar; i++) {
	int i = 1;
	int count = 0;
	while (count < total_bar) {
		if (i >= 90) break;
		if (Close[i] > senkou_span_a[i] && Close[i] > senkou_span_b[i]) {
			up_trend++;
			count++;
		} else if (Close[i] < senkou_span_a[i] && Close[i] < senkou_span_b[i]) {
			down_trend++;
			count++;
		}
		i++;
	}
	
	int trend = up_trend - down_trend;
	
	if (trend > 30) forecast = FC_SHORT;
	else if (trend < -30) forecast = FC_LONG;
	
	
	
}


