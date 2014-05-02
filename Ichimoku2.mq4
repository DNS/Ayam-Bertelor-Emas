//+------------------------------------------------------------------+
//|                                                    Ichimoku2.mq4 |
//|                                Copyright 2014, Daniel Hasudungan |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Hasudungan"
#property link      "http://www.siraitx.com/"
#property version   "2.00"
#property description "Ichimoku Kinko Hyo v2.0.0"
#property strict

#define MAGICNUMBER  06112013

#import "Ayam Bertelor Emas.dll"
void winapi_MessageBoxW (string, string);
#import


int SL = 130;	// 13 pips
int TP = 10000;	// 1000 pips
int TrailingStop = 20;
double lots = 0.01;
double spread = 0.0;
int leverage = 1;
string buf;
int ticket = -1;
int period = PERIOD_H1;

bool is_trade = false;
bool wait_trade = true;
bool first_run = true;
bool detect_profit = false;
bool skip_bar = true;
int count_bar = 0;

double tenkan_sen[30];
double kijun_sen[30];
double senkou_span_a[30];
double senkou_span_b[30];
double chinkou_span[30];

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
	
	for (int i=0; i<30; i++) {
		// shift 1
		tenkan_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_TENKANSEN, i);
		kijun_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_KIJUNSEN, i);
		senkou_span_a[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANA, i);
		senkou_span_b[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANB, i);
		chinkou_span[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_CHINKOUSPAN, i);
	}
	
	
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
		if (count_bar >= 3) {
			skip_bar = false;
			count_bar = 0;
		} else
			count_bar++;
	}
	
	if (OrdersTotal() == 0 
		&& skip_bar == false
		) {
		
		// entry: BUY
		if (Ask > kijun_sen[0]
			&& Ask > senkou_span_a[0] && Ask > senkou_span_b[0]
			//&& tenkan_sen[1] >= tenkan_sen[2] && tenkan_sen[1] > tenkan_sen[3]
			&& chinkou_span[26] > High[26]
			&& chinkou_span[26] > senkou_span_a[26] && chinkou_span[26] > senkou_span_b[26]
			&& chinkou_span[26] != 0.0
			//&& kijun_sen[0] > kijun_sen[10] && kijun_sen[5] > kijun_sen[10]
			) {
			
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			is_trade = true;
		
		// entry: SELL
		} else if (Bid < kijun_sen[0]
			&& Bid < senkou_span_a[0] && Bid < senkou_span_b[0]
			//&& tenkan_sen[1] <= tenkan_sen[2] && tenkan_sen[1] < tenkan_sen[3]
			&& chinkou_span[26] < Low[26]
			&& chinkou_span[26] < senkou_span_a[26] && chinkou_span[26] < senkou_span_b[26]
			&& chinkou_span[26] != 0.0
			//&& kijun_sen[0] < kijun_sen[10] && kijun_sen[5] > kijun_sen[10]
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

