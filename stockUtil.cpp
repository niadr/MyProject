#include "stockUtil.h"
#include <iostream>

using namespace std;
						
Stock::Stock() : _symbol       (),
			_lastDividend      (0),
			_parValue          (0),
			_lastPrice         (0),
			_lastDividendYield (0.0),
			_lastPERatio       (0.0),
			_weightedStockPrice(0.0)
			{}

Stock::Stock(string const& symbol) :
				_symbol            (symbol),
				_lastDividend      (0),
				_parValue          (0),
				_lastPrice         (0),
				_lastDividendYield (0.0),
				_lastPERatio       (0.0),
				_weightedStockPrice(0.0)
				{}
			
Stock::Stock(const char*       symbol, 
 			 int               lastDividend,
			 int               parValue) :
			_symbol            (string(symbol)),
			_lastDividend      (lastDividend),
			_parValue          (parValue),
			_lastPrice         (0),
			_lastDividendYield (0.0),
			_lastPERatio       (0.0),
			_weightedStockPrice(0.0)
			{}


Stock* Stock::clone() const
{
	Stock* newStock = new Stock();
	copyData(newStock);
	return newStock;
}

double Stock::computeDividendYield(int price)
{
	if (price > 0 && _lastDividend >= 0) {
		_lastDividendYield = (double) _lastDividend / price ;
	} else {
		cout << "Cannot compute Dividend Yield for stock " << _symbol
		     << " due to invalid values" << endl;
		_lastDividendYield = 0.0;
	}
	return _lastDividendYield;
}

double Stock::computePERatio(int price)
{
	if (_lastDividend > 0 && price >= 0) {
		_lastPERatio = (double) price / _lastDividend ;
	} else {
		cout << "Cannot compute P/E ratio for stock " << _symbol 
		     << " due to invalid values" << endl;
		_lastPERatio = 0.0;
	}
	return _lastPERatio;
}

double Stock::computeWeightedStockPrice(TradesVec const& trades)
{
	long int sumPriceQuantity = 0;
	long int sumQuantity      = 0;
	time_t   rawTime;
	time(&rawTime);
	for (auto trade : trades) {
		const time_t& tradeTime = trade->timestamp();
		int           price     = trade->price();
		if (tradeTime >= rawTime - 300 && tradeTime <= rawTime) {
			int quantity = trade->quantity();
			sumPriceQuantity += price * quantity;
			sumQuantity      += quantity;
		} 
	}
	
	if (sumQuantity > 0) {
		_weightedStockPrice = (double) sumPriceQuantity / sumQuantity;
	} else {
		cout << "Volume Weighted Stock Price not computed" << endl;
		_weightedStockPrice = 0.0;
	}
	return _weightedStockPrice;
}

void Stock::copyData(Stock* dest) const
{
	if (dest) {
		dest->symbol            (_symbol);
		dest->lastDividend      (_lastDividend);
		dest->parValue          (_parValue);
		dest->lastPrice         (_lastPrice);
		dest->lastDividendYield (_lastDividendYield);
		dest->lastPERatio       (_lastPERatio);
		dest->weightedStockPrice(_weightedStockPrice);
	}
}

PreferredStock::PreferredStock() : Stock(),
								   _fixedDividend(0)

								{}
PreferredStock::PreferredStock(string const&    symbol):
								Stock         (symbol),
								_fixedDividend(0)
								{}
				
PreferredStock::PreferredStock(const char*    symbol,
								int           lastDividend,
								int           parValue,
								int           fixedDividend) :
								Stock         (symbol, lastDividend, parValue),
								_fixedDividend(fixedDividend)
								{}


Stock* PreferredStock::clone() const
{
	Stock* newStock = new PreferredStock();
	copyData(newStock);
	return newStock;
}
		
double PreferredStock::computeDividendYield(int price)
{
	int parVal = parValue();
	if (price > 0 && _fixedDividend >= 0 && parVal >= 0)  {
		double yield = (double) (0.01 * _fixedDividend * parVal) / price;
		lastDividendYield(yield);
	} else {
		cout << "Cannot compute Dividend Yield for stock " << symbol() 
		     << " due to invalid values" << endl;
		lastDividendYield(0.0);
	}
	return lastDividendYield();
}

void PreferredStock::copyData(Stock* dest) const
{
	if (dest) {
		Stock::copyData(dest);
		static_cast<PreferredStock*>(dest)->fixedDividend(_fixedDividend);
	}
}

Trade::Trade() :
	  	_symbol  (""),
		_price   (0),
		_quantity(0),
		_buy     (false)
{
	time(&_timestamp);
}

Trade::Trade(Trade const& t) :
			_symbol    (t.symbol()),
			_price     (t.price()),
			_quantity  (t.quantity()),
			_buy       (t.buying()),
			_timestamp(t.timestamp())
		{}
		
Trade::Trade(const char* symbol,
			  int   	 price,
		      int        quantity,
		  	  bool       buy) :
		  	_symbol  (string(symbol)),
			_price   (price),
			_quantity(quantity),
			_buy     (buy)
{
	time(&_timestamp);
}

void Stock::printInfo() const
{
	if (!_symbol.empty()) {
		cout << _symbol  << "\t\t Common"
			 << "\t \t " << _lastDividend
		     << "\t"     << "--------"
		     << "\t"     << _parValue 
			 << "\t    " << _lastPrice 
			 << "\t    " << _lastDividendYield 
			 << "   "    << _lastPERatio
			 << "\t\t"   << _weightedStockPrice
			 << endl;
	}
}

void PreferredStock::printInfo() const
{
	string name = symbol();
	if (!name.empty()) {	
		cout << name     << "\t\t Preferred"
		     << "\t "    << lastDividend() 
		     << "\t"     << _fixedDividend << "%"
		     << "\t\t"   << parValue() 
		     << "\t    " << lastPrice() 
			 << "\t    " << lastDividendYield()
			 << "   "    << lastPERatio()
			 << "\t\t"   << weightedStockPrice()
			 << endl;
	}
}

void Trade::printInfo() const
{
	if (!_symbol.empty()) {
		const char* buyOrSell = _buy ? "BUY" : "SELL";
		cout <<            _symbol
			 << "\t"    << _price
			 << "\t"    << _quantity 
			 << "\t"    << buyOrSell
			 <<	"\t"    << _timestamp
			 << "\t"    << ctime(&_timestamp)
			 << endl;
	}
}
