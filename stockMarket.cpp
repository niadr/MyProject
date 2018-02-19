#include "stockMarket.h"
#include <iostream>
#include <cmath>

using namespace std;

StockMarket::StockMarket():
				_name         (NULL),
				_location     (NULL),
				_country      (NULL),
				_geometricMean(0.0),
				_stocks       (),
				_trades       ()
				{}
	
StockMarket::StockMarket(const char* name,
						const char* location,
						const char* country):
						_name         (name),
						_location     (location),
						_country      (country),
						_geometricMean(0.0),
						_stocks       (),
						_trades       ()
						{}

StockMarket::~StockMarket()						
{
	// Cleanup stocks memory
	for (auto stock : _stocks) {
		delete stock.second;
	}
	
	// Cleanup trades memory
	for (auto iter : _trades) {
		TradesVec stockTrades = iter.second;
		for (auto trade : stockTrades) { 
			delete trade;
		}
	}
	_stocks.clear();
	_trades.clear();
}

bool StockMarket::addStock(const Stock* stock)
{
	bool result = false;
	if (stock) {
		string symbol = stock->symbol();
		if (!symbol.empty() && 
			_stocks.find(symbol) == _stocks.end()) {
			// Take ownership of stock memory
			Stock* newStock = stock->clone();
			if (newStock) {
				_stocks[symbol] = newStock;
				result = true;
			}
		} else {
			cout << "Stock '" << stock->symbol() << "' not added to stock markert '"
				<< _name << "'" << endl;
		}
	}
	return result;
}

bool StockMarket::addTrade(const Trade* trade)
{
	bool result = false;
	if (trade) {
		// Make sure stock symbol is already registered
		// Otherwise cannot trade with unregistered stock
		string symbol = trade->symbol();
		if (!symbol.empty()) {
			 StocksMap::iterator iter = _stocks.find(symbol);
			if (iter != _stocks.end()) {
				// Take ownership of trade memory
				Trade* newTrade = new Trade(*trade);
				if (newTrade) {
					TradesMapIter it = _trades.find(symbol);
					if (it == _trades.end()) {
						// Stock is registered but not yet traded
						TradesVec stockTrades;
						stockTrades.push_back(newTrade);
						_trades[symbol] = stockTrades;
					} else {
						// Stock has been previously traded
						(*it).second.push_back(newTrade);
					}
					// Eventually update the stock price
					Stock *stock = (*iter).second;
					stock->lastPrice(newTrade->price());
					result = true;
				}  // newTrade
			} // stocks iter
		} else {
			cout << "Cannot add trade '" << symbol 
			    << "' (invalid symbol or unregistered stock)" << endl;
		}
	} else {
		cout << "Invalid trade pointer " << endl;
	}
	return result;
}

void StockMarket::computeStockValues()
{
	double geometricMeanAcc = 1.0;  // accumulate values used to compute Geometric Mean
	int    numTrades        = 0;    // Only traded stocks will be used to compute the Geometric Mean
	for (auto iter : _stocks) {
		Stock* stock = iter.second;
		string symbol = stock->symbol();
		int    price  = stock->lastPrice();
				 
		double dividendYied = stock->computeDividendYield(price); 
		double PERatio      = stock->computePERatio(price); 
			
		TradesMapIter it = _trades.find(symbol);
		if (it != _trades.end()) {
			numTrades++;
			double vwapValue = stock->computeWeightedStockPrice((*it).second);
			geometricMeanAcc *= vwapValue;
		} else {
			cout << "Stock '" << symbol << "' not yet traded on stock '" << _name << "'" << endl; 
		}
	}
	if (numTrades > 0) {
		_geometricMean = (double) std::pow(geometricMeanAcc,  (double) (1.0 / numTrades));
	}
}

const Stock* StockMarket::findStock(const char* symbol) const
{
	Stock* result = NULL;
	 StocksMap::const_iterator iter = _stocks.find(string(symbol));
	 if (iter != _stocks.end()) {
	 	result = (*iter).second;
	 } 
	 return result;
}

const TradesVec* StockMarket::getTrades(const char* symbol) const
{
	const TradesVec* result = NULL;
	TradesMap::const_iterator iter = _trades.find(string(symbol));
	if (iter != _trades.end()) {
		result =  &(*iter).second;
	} 
	return result;
}

void StockMarket::printInfo() const
{
	if (_name && *_name) {
		cout << "\nPRINTING INFOS AND DATA FOR STOCK: " << _name;
	}
	if (_location && *_location) {
		cout << ", " << _location;
	}
	if (_country && *_country) {
		cout << ", " << _country;
	}
	cout << endl << endl;
	
	printTrades();
	printStockValues();
	
	cout << "\nThe Goemetric Mean of this stock market is " << _geometricMean << endl;
}
	
void StockMarket::printTrades() const
{
	cout << "\nTRADING INFORMATION \n" << endl;
	cout << "SYMBOL \tPRICE \tQTY \tBUY_SELL TIMESTAMP \tLOCALTIME" << endl;
	cout << "-------------------------------------------------------------" << endl;
	
	for (auto iter : _trades) {
		TradesVec stockTrades = iter.second;
		for (TradesVec::const_iterator i = stockTrades.begin(),
			e = stockTrades.end(); i != e; ++i) {
			(*i)->printInfo();
		}
	}
	cout << endl;
}

void StockMarket::printStockValues() const
{
	cout << "\nStock Symbol \tType \tLast Dividend  Fixed Dividend \tPar Value   Price   Yield  Ratio  WeightedStockPrice" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------" << endl;

	for (auto iter2 : _stocks) {
		Stock* stock = iter2.second;
		stock->printInfo();
	}
	cout << endl;
}
    	
