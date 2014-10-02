//+------------------------------------------------------------------+
//|                                                          NNR.mq4 |
//|                                    Copyright 2014, Daniel Sirait |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Sirait"
#property link      "http://www.siraitx.com/"
#property version   "1.00"
#property description "Neural Networks Regression v0.0.1"
#property strict


#define MAGICNUMBER  22072014

#import "Ayam Bertelor Emas.dll"

void winapi_MessageBoxW (string, string);
void winapi_MessageBoxA (string, string);

void initPriceLog (string);
void addPriceLog (double);
void flushPriceLog ();

#import


int SL = (14 - 1.2) * 10;	// 13, 30, 40 pips SL ?
int TP = (1000) * 10;	// 1000 pips
int TrailingStop = 20;
double lots = 0.01;
double spread = 0.0;
int leverage = 1;	// set normal leverage value : 1
string buf;
int ticket = -1;
int period = PERIOD_M1;

double m_eurusd;
double m_gbpusd;

enum _MODE {
	LONG,
	SHORT
} mode;

double max_loss = 0.0;


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

	//initPriceLog(Symbol());

	return(INIT_SUCCEEDED);
}



void OnDeinit (const int reason) {
	//DoubleToString(High[0])
	//IntegerToString(Bars)
	//winapi_MessageBoxW("Debug: " + DoubleToString(High[Bars-1]) + "\n", "Title!");
	
	//flushPriceLog();
	
	
}



void OnTick () {
	if (Bars < 200) return;
	if (Volume[0] != 1) return;
	
	//addPriceLog(double);
	
	//iClose("GBPUSD",PERIOD_H1,0);
	m_eurusd = iMomentum("EURUSD", PERIOD_H1, 200, PRICE_CLOSE, 1) - 100.0;
	m_gbpusd = iMomentum("GBPUSD", PERIOD_H1, 200, PRICE_CLOSE, 1) - 100.0;
	//Comment(m_eurusd);
	// <?xml version="1.0" encoding="utf-8"?>
	if (OrdersTotal() == 0) {
		winapi_MessageBoxW("m_eurusd: " + DoubleToString( m_eurusd ) +
			"\nm_gbpusd: " + DoubleToString( m_gbpusd )
			, "Title!");
		
	
	
	}
	//winapi_MessageBoxW("Close: " + DoubleToString(Close[0]), "Title!");
	
}


