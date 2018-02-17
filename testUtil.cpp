#include "testUtil.h"
#include "stockMarket.h"
#include "stockUtil.h"
#include <iostream>

using namespace std;

void Tester::addStockData(StockMarket& stockMarket)
{
	Stock stock1;
	stock1.symbol("TEA");
	stock1.lastDividend(0);
	stock1.parValue(100);
	stock1.lastPrice(20);
		
	Stock stock2("POP", 8, 100);
	stock2.lastPrice(25);
	
	Stock stock3("ALE");
	stock3.lastDividend(23);
	stock3.parValue(60);
	stock3.lastPrice(-10);
	
	PreferredStock stock4("GIN", 0, 0, 0);
	stock4.lastDividend(8);
	stock4.fixedDividend(2);
	stock4.parValue(100);
	stock4.lastPrice(20);
	
	Stock stock5("JOE", 13, 250);
	stock5.lastPrice(0);
	
	// Register stocks to market
	stockMarket.addStock(&stock1);
	stockMarket.addStock(&stock2);
	stockMarket.addStock(&stock3);
	stockMarket.addStock(&stock4);
	stockMarket.addStock(&stock5);

	// Register some trades
	Trade trade1;
	trade1.symbol("TEA");
	trade1.price(60);
	trade1.buying(false);  // selling
	trade1.quantity(50);
	
	time_t rawtime;
	time(&rawtime);  // current time in seconds
	int price      = 45;
	int quantity   = 80;
	const bool buy = true;
	Trade trade2("ALE",  price,    quantity,      buy);
	Trade trade3("ALE",  price+10, quantity+100,  buy);
	Trade trade4("ALE",  price-5,  quantity+200, !buy);
	Trade trade5("ALE",  price+2,  150,           buy);
	Trade trade6("ALE",  price,    quantity,     !buy);
	Trade trade7("ALOA", price,    quantity,     !buy); // stock not registered!
	Trade trade8("GIN",  60,       250,           buy);
	
	trade3.timestamp(rawtime-60);
	trade4.timestamp(rawtime-200);
	trade5.timestamp(rawtime-300);
	trade6.timestamp(rawtime-400); 
	// trade6 should be excluded from the computation
	// of the 'Volume Weighted Stock Price' on trades of past 5 min
	
	stockMarket.addTrade(&trade1);
	stockMarket.addTrade(&trade2);
	stockMarket.addTrade(&trade3);
	stockMarket.addTrade(&trade4);
	stockMarket.addTrade(&trade5);
	stockMarket.addTrade(&trade6);
	stockMarket.addTrade(&trade7);
	stockMarket.addTrade(&trade8);
}

void Tester::check(StockMarket& stockMarket)
{
	int numPasses = 0;
	int numFails  = 0;
	
    // For each stock check following values
    // Stock symbol, Last dividend, Par Value
    // Last price, Dividend Yield, P/E Ratio and Volume Weighted Stock Price
	if (checkStock("JOE", 13, 250, 0, 0.00, 0.00, 0.00, stockMarket)) { 
		numPasses++;
	} else {
		numFails++;
	}
	if (checkStock("GIN", 8, 100, 60, 0.0333333, 7.50, 60, stockMarket)) {
		numPasses++;
	} else {
		numFails++;
	}
	if (checkStock("ALE", 23, 60, 45, 0.511111, 1.95652, 46.0145, stockMarket)) {
		numPasses++;	
	} else {
		numFails++;
	}
	if(checkStock("POP", 8, 100, 25, 0.32, 3.125, 0, stockMarket)) {
		numPasses++;
	} else {
		numFails++;
	}
	if (checkStock("TEA", 0, 100, 60, 0.00, 0.00, 60.00, stockMarket)){
		numPasses++;
	} else {
		numFails++;
	}

	// Check Geometric Mean
	if (stockMarket.geometricMean() == 54.9202) {
		numPasses++;
	} else {
		cout << "Test for Geometric Mean fails" << endl;
		numFails++;
	}
	
	// Tests may fails due to precision differences when comparing double numbers
	
	cout << "----------------------------------------------" << endl;
	cout << numPasses << " Tests pass" << endl;
	cout << numFails <<  " Tests fails :"  << numFails << endl;
}

bool Tester::checkStock(const char*  symbol,
						int          lastDividend,
						int          parValue,
						int          lastPrice,
						double       dividendYield,
						double       PERatio,
						double       vwap,
						StockMarket& stockMarket)
{
	bool result = false;
	const Stock* stock = stockMarket.findStock(symbol);
	if (stock && 
		stock->lastDividend()       == lastDividend  &&
		stock->parValue()           == parValue      &&
		stock->lastPrice()          == lastPrice     &&
		stock->lastDividendYield()  == dividendYield &&
		stock->lastPERatio()        == PERatio       &&
		stock->weightedStockPrice() == vwap) {
		result = true;
 	} else {
 		cout << "Test for stock symbol " <<  symbol << " Fails" << endl;
 	}
	return result;
}
