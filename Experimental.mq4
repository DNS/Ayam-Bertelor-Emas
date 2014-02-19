//+------------------------------------------------------------------+
//|														 Ayam Bertelor Emas.mq4 |
//|										  Copyright 2013, Daniel Hasudungan |
//|											  http://www.imbalancegames.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2013, Daniel Hasudungan"
#property link		"http://www.siraitx.com/"

#define MAGICNUMBER  06112013


#import "Ayam Bertelor Emas.dll"
//#import "bebek.dll"
void ayam_init (int _period);
int ayam_start (double tick, int);
void ayam_deinit ();
void ayam_mt4stoploss ();

int SL = 50;
int TP = 15000;
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

int init () {
	spread = MarketInfo(Symbol(), MODE_SPREAD);
	
	ayam_init(2000);		// period: 100 tick
	
	//MessageBox("start", "qq", 1);
	return(0);
}

int deinit () {
	ayam_deinit();
	return(0);
}


int start () {
	int result;
	
	
	
	if (is_trade == true && OrdersTotal() == 0) {
		ayam_mt4stoploss();
		is_trade = false;
	}
	
	if (OrdersTotal() == 0) {
		
		result = ayam_start(Close[0], ANALYZE_OPEN);
		if (result == MARKET_OPEN_BUY) {
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			is_trade = true;
		} else if (result == MARKET_OPEN_SELL) {
			ticket = OrderSend(Symbol(), OP_SELL, lots, Bid, 3, Ask+SL*Point, Ask-TP*Point, NULL, MAGICNUMBER, 0, Red);
			is_trade = true;
		}
	} else {
		bool ret;
		result = ayam_start(Close[0], ANALYZE_CLOSE);
		OrderSelect(ticket, SELECT_BY_TICKET);
		if (result == MARKET_CLOSE_SELL_OK) {
			
			ret = OrderClose(ticket, OrderLots(), Ask, 0, White);		// Close SELL
			if (ret == false)
				Comment("Error Close Sell: " + GetLastError());
			
				
			is_trade = false;
			
		} else if (result == MARKET_CLOSE_BUY_OK) {
			
			ret = OrderClose(ticket, OrderLots(), Bid, 0, White);		// Close BUY
			if (ret == false)
				Comment("Error Close Buy: " + GetLastError());
				
			is_trade = false;
		}
	}
	
	
	return(0);
}



