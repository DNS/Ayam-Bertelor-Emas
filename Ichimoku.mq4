//+------------------------------------------------------------------+
//|                                                     Ichimoku.mq4 |
//|                                Copyright 2014, Daniel Hasudungan |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Hasudungan"
#property link      "http://www.siraitx.com/"

#define MAGICNUMBER  06112013

int SL = 200;
int TP = 4000;	// 400 pips
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
bool wait_trade = true;
bool first_run = true;
bool detect_profit = false;

// if price up/below kumo
int UP_KUMO = 1;
int DOWN_KUMO = 2;

int kumo_state;


double tenkan_sen[26];
double kijun_sen[26];
double senkou_span_a[26];
double senkou_span_b[26];
double chinkou_span[26];
double macd_main;
double macd_signal;

int init () {
	if (IsTradeAllowed()) {
		// msgbox
	}
	
	spread = MarketInfo(Symbol(), MODE_SPREAD);
	
	//MessageBox("start", "qq", 1);
	return(0);
}

int deinit () {
	return(0);
}


int start () {
	
	// hit SL/TP
	if (is_trade == true && OrdersTotal() == 0) {
		is_trade = false;
		ticket = -1;
		wait_trade = true;
		detect_profit = false;
	}
	
	for (int i=0; i<26; i++) {
		// shift 1
		tenkan_sen[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_TENKANSEN, i+1);
		kijun_sen[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_KIJUNSEN, i+1);
		senkou_span_a[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_SENKOUSPANA, i+1);
		senkou_span_b[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_SENKOUSPANB, i+1);
		chinkou_span[i] = iIchimoku(Symbol(), PERIOD_H1, 9, 26, 52, MODE_CHINKOUSPAN, i+1);
	}
	
	macd_main = iMACD(Symbol(), 0, 12, 26, 9, PRICE_CLOSE, MODE_MAIN, 1);
	macd_signal = iMACD(Symbol(), 0, 12, 26, 9, PRICE_CLOSE, MODE_SIGNAL, 1);
	
	if (first_run) {
		if (Close[1] > senkou_span_a[0] && Close[1] > senkou_span_b[0]) {
			kumo_state = UP_KUMO;
			wait_trade = true;
			first_run = false;
		} else if (Close[1] < senkou_span_a[0] && Close[1] < senkou_span_b[0]) {
			kumo_state = DOWN_KUMO;
			wait_trade = true;
			first_run = false;
		}
		
		return;
	}
	
	if (wait_trade) {
		if (kumo_state == DOWN_KUMO && Close[1] > senkou_span_a[0] && Close[1] > senkou_span_b[0]) {
			wait_trade = false;
			kumo_state = UP_KUMO;
		} else if (kumo_state == UP_KUMO && Close[1] < senkou_span_a[0] && Close[1] < senkou_span_b[0]) {
			wait_trade = false;
			kumo_state = DOWN_KUMO;
		} else {
			return;
		}
	}
	
	
	if (OrdersTotal() == 0) {
		if (Volume[0] > 100) return;
		
		// open trade
		// entry: BUY
		if (Close[1] > kijun_sen[0] && Close[1] > senkou_span_a[0] && Close[1] > senkou_span_b[0] &&
			chinkou_span[25] > Close[25] &&	Low[1] > senkou_span_a[0] 
			&& macd_main > macd_signal
			//&& macd_signal < 0
			) {
			/*double stop_loss;
			if (senkou_span_a[0] > senkou_span_b[0]) {
				stop_loss = senkou_span_a[0];
			} else if (senkou_span_a[0] > senkou_span_b[0]) {
				stop_loss = senkou_span_b[0];
			}
			*/
			//ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, stop_loss, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			if (ticket != -1) is_trade = true;
			
		}
	} else {
		// close trade, TP, SL
		
		OrderSelect(ticket, SELECT_BY_TICKET);
		
		// exit: BUY
		if (macd_main < macd_signal) {
			OrderClose(ticket, OrderLots(), Bid, 0, White);
			ticket = -1;
			wait_trade = true;
			is_trade = false;
		}
		
		
		// Detect Profit
		//if () 
	}
	
	
	
	
	
	
	return(0);
}



