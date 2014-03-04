//+------------------------------------------------------------------+
//|                                                     Ichimoku.mq4 |
//|                                Copyright 2014, Daniel Hasudungan |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Hasudungan"
#property link      "http://www.siraitx.com/"

#define MAGICNUMBER  06112013

int SL = 150;
int TP = 1500;
int TrailingStop = 20;
double lots = 0.01;
double spread = 0.0;
string buf;
int ticket = -1;
int period = PERIOD_M1;

int MARKET_OPEN_FLAT = 0;
int MARKET_OPEN_BUY = 1;
int MARKET_OPEN_SELL = 2;

int MARKET_CLOSE_NO = 3;
int MARKET_CLOSE_BUY_OK = 4;
int MARKET_CLOSE_SELL_OK = 5;

int ANALYZE_OPEN = 1001;
int ANALYZE_CLOSE = 1002;

bool is_trade = false;


double tenkan_sen[27];
double kijun_sen[27];
double senkou_span_A[27];
double senkou_span_B[27];
double chinkou_span[27];


int init () {
	spread = MarketInfo(Symbol(), MODE_SPREAD);
	
	
	//MessageBox("start", "qq", 1);
	return(0);
}

int deinit () {
	return(0);
}


int start () {
	int result;
	
	
	
	if (is_trade == true && OrdersTotal() == 0) {
		is_trade = false;
		ticket = -1;
	}
	
	for (int i=0; i<27; i++) {
		tenkan_sen[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_TENKANSEN, i);
		kijun_sen[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_KIJUNSEN, i);
		senkou_span_A[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_SENKOUSPANA, i);
		senkou_span_B[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_SENKOUSPANB, i);
		chinkou_span[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_CHINKOUSPAN, i);
	}
	
	if (OrdersTotal() == 0) {
		if (Volume[0] > 100) return;
		
		// open trade
		// entry: BUY
		if (Close[1] > kijun_sen[1] && Close[1] > senkou_span_A[1] && chinkou_span[26] > Close[1]) {
			
		}
		
		
	} else {
		// close trade
		
	}
	
	
	return(0);
}



