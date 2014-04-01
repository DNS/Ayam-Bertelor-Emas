//+------------------------------------------------------------------+
//|                                                     Ichimoku.mq4 |
//|                                   Copyright 2014, Daniel Sirait  |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+

#property copyright "Copyright 2014, Daniel Sirait"
#property link      "http://www.siraitx.com/"

#define MAGICNUMBER  06112013


#import "Ayam Bertelor Emas.dll"
void MsgBox (string, string);


int SL = 130;	// 13 pips
int TP = 10000;	// 1000 pips
int TrailingStop = 20;
double lots = 0.01;
double spread = 0.0;
string buf;
int ticket = -1;
int period = PERIOD_H1;

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
int FLAT_KUMO = 3;

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
	Comment(spread);
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
		kumo_state = FLAT_KUMO;
	}
	
	for (int i=0; i<26; i++) {
		// shift 1
		tenkan_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_TENKANSEN, i);
		kijun_sen[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_KIJUNSEN, i);
		senkou_span_a[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANA, i);
		senkou_span_b[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_SENKOUSPANB, i);
		chinkou_span[i] = iIchimoku(Symbol(), period, 9, 26, 52, MODE_CHINKOUSPAN, i);
	}
	
	//macd_main = iMACD(Symbol(), period, 12, 26, 9, PRICE_CLOSE, MODE_MAIN, 1);
	//macd_signal = iMACD(Symbol(), period, 12, 26, 9, PRICE_CLOSE, MODE_SIGNAL, 1);
	
	if (first_run) {
		if (Close[1] > senkou_span_a[1] && Close[1] > senkou_span_b[1]) {
			kumo_state = UP_KUMO;
			wait_trade = true;
			first_run = false;
		} else if (Close[1] < senkou_span_a[1] && Close[1] < senkou_span_b[1]) {
			kumo_state = DOWN_KUMO;
			wait_trade = true;
			first_run = false;
		}
		
		return 0;
	}
	
	
	// BUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (kumo_state != FLAT_KUMO && ((Close[1] < senkou_span_a[1] && Close[1] > senkou_span_b[1]) || 
		(Close[1] > senkou_span_a[1] && Close[1] < senkou_span_b[1])) ) {
		kumo_state = FLAT_KUMO;
		wait_trade = true;
	}	
		
	if (wait_trade) {
		if (kumo_state != UP_KUMO && Close[1] > senkou_span_a[1] && Close[1] > senkou_span_b[1]) {
			kumo_state = UP_KUMO;
			wait_trade = false;
			//MsgBox("WAIT_FALSE DOWN", "caption");
		} else if (kumo_state != DOWN_KUMO && Close[1] < senkou_span_a[1] && Close[1] < senkou_span_b[1]) {
			kumo_state = DOWN_KUMO;
			wait_trade = true;
			//MsgBox("WAIT_FALSE UP", "caption");
		} else {
			//return;
		}
	} else {
		
	}
	
	
	if (OrdersTotal() == 0) {
		//if (Volume[0] > 100) return;
		
		
		double senkou_diff = MathAbs(senkou_span_a[0] - senkou_span_b[0]);
		bool sideway_break = false;
		
		if (senkou_diff < 0.00070 && senkou_span_a[0] > senkou_span_b[0]) {
			if ((Close[0] - senkou_span_a[0]) > 0.0015)
				sideway_break = true;
		} else if (senkou_diff < 0.00070 && senkou_span_a[0] < senkou_span_b[0]) {
			if ((Close[0] - senkou_span_b[0]) > 0.0015)
				sideway_break = true;
		}
		
		// open trade
		// entry: BUY
		if (//Close[1] > kijun_sen[1] 
			Close[0] > kijun_sen[0] 
			&& Close[0] > senkou_span_a[0] && Close[0] > senkou_span_b[0]  
			&& tenkan_sen[1] >= tenkan_sen[2] && tenkan_sen[1] >= tenkan_sen[3]
			&& (senkou_diff > 0.00070 || sideway_break)
			//&& tenkan_sen[1] > kijun_sen[1]
			//chinkou_span[25] > Close[25] &&
			//Open[1] > senkou_span_a[1] && 
			//Open[1] > senkou_span_b[1] 
			&& (Close[0] - kijun_sen[0]) > 0.0012
			&& (wait_trade == false ||
				(Low[0] < senkou_span_a[0] && Low[0] < senkou_span_b[0] &&
				sideway_break)
				)
				
			) {
			
			
			//ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, stop_loss, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			ticket = OrderSend(Symbol(), OP_BUY, lots, Ask, 3, Bid-SL*Point, Bid+TP*Point, NULL, MAGICNUMBER, 0, Blue);
			if (ticket != -1) is_trade = true;
			/*
			if (kumo_state == DOWN_KUMO)
				MsgBox("DOWN_KUMO", "caption");
			else if (kumo_state == UP_KUMO)
				MsgBox("UP_KUMO", "caption");
			*/
		}
		
	} else {
		// close trade, TP, SL
		
		OrderSelect(ticket, SELECT_BY_TICKET);
		
		// Detect Profit
		if (Open[0] > (OrderOpenPrice()+0.00012)) 
			detect_profit = true;
		else
			detect_profit = false;
		
		// exit: BUY
		// TP
		if (OrderType() == OP_BUY) {
			if (//macd_main < macd_signal
			//|| chinkou_span[25] < Low[25]
			Close[0] < kijun_sen[0] &&
			detect_profit == true &&
			(Close[0] > OrderOpenPrice()+0.00012+0.00070)
			) {
				OrderClose(ticket, OrderLots(), Bid, 0, White);
				ticket = -1;
				wait_trade = true;
				is_trade = false;
				//MsgBox("CLOSE: WAIT TRUE", "caption");
				kumo_state = FLAT_KUMO;
				detect_profit = false;
			}
			
			
		} // end BUY
			
		
		// SL
		/*
		double stop_loss;
		if (senkou_span_a[0] < senkou_span_b[0]) {
			stop_loss = senkou_span_a[0];
		} else if (senkou_span_a[0] > senkou_span_b[0]) {
			stop_loss = senkou_span_b[0];
		}
		if (Close[0] <= (stop_loss + 0.0000) ) {
			OrderClose(ticket, OrderLots(), Bid, 0, White);
			ticket = -1;
			//wait_trade = true;
			is_trade = false;
			//MsgBox("KUMO HIT", "caption");
		}
		*/
		
	}
	
	
	
	
	
	return(0);
}



