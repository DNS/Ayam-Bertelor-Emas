//+------------------------------------------------------------------+
//|                                       Price Rebound Analyzer.mq4 |
//|                                    Copyright 2014, Daniel Sirait |
//|                                          http://www.siraitx.com/ |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, Daniel Sirait"
#property link      "http://www.siraitx.com/"
#property version   "1.00"
#property description "Price Rebound Analyzer v0.0.1"
#property strict

#define MAGICNUMBER  22072014

#import "Ayam Bertelor Emas.dll"
void winapi_MessageBoxW (string, string);
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
	
	return(INIT_SUCCEEDED);
}



void OnDeinit (const int reason) {
	//DoubleToString(High[0])
	//IntegerToString(Bars)
	//winapi_MessageBoxW("Debug: " + DoubleToString(High[Bars-1]) + "\n", "Title!");
	
	bool start;
	int i, j;
	double loss = 0.0, loss_tmp = 0.0;
	
	for (i=Bars-1; i>=0; i--) {
		
		start = false;
		loss = 0.0;
		
		for (j=i-1; j>=0; j--) {
			
			// run only once per "i" loop
			if (start == false) {
				start = true;
				// loss long
				if (Low[j] < Close[i]) {
					mode = SHORT;
				}
				// loss short
				else if (High[j] > Close[i]) {
					mode = LONG;
				}
			// after
			} else {
				if (mode == SHORT && Low[j] < Close[i]) {
					loss_tmp = Close[i] - Low[j];
					if (loss_tmp > loss) loss = loss_tmp;
				} else if (mode == LONG && High[j] > Close[i]) {
					loss_tmp = High[j] - Close[i];
					if (loss_tmp > loss) loss = loss_tmp;
				}
				
				//winapi_MessageBoxW("max_loss: " + DoubleToString(loss*1E+4) + " pips\n", "Title!");
				
				if (mode == SHORT && High[j] > Close[i]) {
					if (loss > max_loss) max_loss = loss;
					break;
				} else if (mode == LONG && Low[j] < Close[i]) {
					if (loss > max_loss) max_loss = loss;
					break;
				}
			
			
			}
			
			
		}
	}
	
	
	winapi_MessageBoxW("max_loss: " + DoubleToString(max_loss*1E+4) + " pips\n", "Title!");
	
}


void OnTick () {
	//if(Bars<500) return;
	
	
	
}


