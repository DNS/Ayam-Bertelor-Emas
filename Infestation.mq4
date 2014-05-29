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


int SL = 130;	// 13 pips
int TP = 10000;	// 1000 pips
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
	FC_LONG = 2
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
		//winapi_MessageBoxW("Trade not allowed!, please consult your broker.", "Error!");
		MessageBox("Trade not allowed!, please consult your broker.", "DEBUG");
	}
	
	//winapi_MessageBoxW("OnTester()\n Back init", "DEBUG");
	
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
		tenkan_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_TENKANSEN, i);
		kijun_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_KIJUNSEN, i);
		senkou_span_a[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANA, i);
		senkou_span_b[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANB, i);
		chinkou_span[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_CHINKOUSPAN, i);
		sd[i] = iStdDev(Symbol(), period, 20, 0, MODE_SMA, PRICE_CLOSE, i);
		ma[i] = iMA(Symbol(), period, 20, 0, MODE_SMMA, PRICE_CLOSE, i);
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
	}
	
	
	if (Volume[0] == 1 && skip_bar == true) {
		//winapi_MessageBoxW("skip_bar", "DEBUG");
		if (count_bar >= 1) {
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
			forecast == FC_LONG
			
			//Ask > kijun_sen[0]
			&& Ask > senkou_span_a[0] && Ask > senkou_span_b[0]
			//&& ma[1] > ma[2]
			//&& tenkan_sen[1] >= tenkan_sen[2] && tenkan_sen[1] > tenkan_sen[3]
			//&& chinkou_span[26] > High[26]
			//&& chinkou_span[26] > senkou_span_a[26] && chinkou_span[26] > senkou_span_b[26]
			//&& chinkou_span[26] != 0.0
			//&& kijun_sen[0] > kijun_sen[10] && kijun_sen[5] > kijun_sen[10]
			//&& (sd[1]*k <= sd[0] || sd[2]*k <= sd[0] || sd[3]*k <= sd[0] || sd[4]*k <= sd[0] || sd[5]*k <= sd[0]
				 //|| sd[6]*k <= sd[0] || sd[7]*k <= sd[0] || sd[8]*k <= sd[0]
				//)
			) {
			
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			is_trade = true;
		
		// entry: SELL
		} else if (
			forecast == FC_SHORT
			//Bid < kijun_sen[0]
			//&& ma[1] < ma[2]
			&& Bid < senkou_span_a[0] && Bid < senkou_span_b[0]
			//&& tenkan_sen[1] <= tenkan_sen[2] && tenkan_sen[1] < tenkan_sen[3]
			//&& chinkou_span[26] < Low[26]
			//&& chinkou_span[26] < senkou_span_a[26] && chinkou_span[26] < senkou_span_b[26]
			//&& chinkou_span[26] != 0.0
			//&& kijun_sen[0] < kijun_sen[10] && kijun_sen[5] > kijun_sen[10]
			//&& (sd[1]*k <= sd[0] || sd[2]*k <= sd[0] || sd[3]*k <= sd[0] || sd[4]*k <= sd[0] || sd[5]*k <= sd[0]
				 //|| sd[6]*k <= sd[0] || sd[7]*k <= sd[0] || sd[8]*k <= sd[0]
				//)
			) {
			
			//winapi_MessageBoxW(DoubleToStr(chinkou_span[26], 5), "DEBUG");
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
		
		if (OrderType() == OP_BUY && detect_profit && pl > 0.0007) {
			if (Bid < kijun_sen[0]) {
				OrderClose(ticket, OrderLots(), Bid, 0, White);
				ticket = -1;
				skip_bar = true;
				is_trade = false;
				count_bar = 0;
			}
		
		} else if (OrderType() == OP_SELL && detect_profit && pl > 0.0007) {
			if (Ask > kijun_sen[0]) {
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
	int down_trend = 0, up_trend = 0;
	int total_bar = 84;					//12 * 7;
	
	for (int i=1; i<total_bar; i++) {
		if (Close[i] > senkou_span_a[i] && Close[i] > senkou_span_b[i]) {
			up_trend++;
		} else if (Close[i] < senkou_span_a[i] && Close[i] < senkou_span_b[i]) {
			down_trend++;
		} else {
			if (up_trend > down_trend) up_trend++;
			else if (up_trend < down_trend) down_trend++;
		}
	}
	
	// search for reversal
	if (up_trend > down_trend) {
		forecast = FC_SHORT;
	} else if (down_trend > up_trend) {
		forecast = FC_LONG;
	} else {
		forecast = FC_FLAT;
	}
		
	
	
	//MessageBox("txt", "DEBUG");
	
	/*if (forecast == FC_SHORT) 
		winapi_MessageBoxW("txt", "DEBUG");
	else if (forecast == FC_LONG) 
		winapi_MessageBoxW("txt", "DEBUG");
		*/
	
}


