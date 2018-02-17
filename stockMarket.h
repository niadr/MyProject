#ifndef _STOCK_MARKET_H_
#define _STOCK_MARKET_H

#include "stockUtil.h"

//! Class to hold stock market information and data
class StockMarket
{
	public:
		StockMarket();
		StockMarket(const char* name,
					const char* location,
					const char* country);
		virtual ~StockMarket();
			
		//! Accessing
		const char* getName()        const { return _name;     }
		const char* getLocation()    const { return _location; }
		const char* getCountry()     const { return _country;  }
		const double geometricMean() const { return _geometricMean;};
		
		StocksMap const&  stocks() const { return _stocks; }
		TradesMap const&  trades() const { return _trades; }
		
		//! Setting
		void setName(const char* name) {
			_name = name;
		}
		void setLocation(const char* location) {
			_location = location;
		}
		void setCountry(const char* country) {
			_country = country;
		}
		
		//! Add a stock to this stock market
		//! Returns True if successfully added, otherwise false
		bool  addStock(const Stock* stock);
			
		//! Add a trade to the stock market
		//! Condition: Valid trade pointer, valid stock symbol for the trade and
		//! Stock being traded should be already registered to the stock market
		//! Returns true if trade added, otherwise 'false'
		bool addTrade(const Trade* trade);
		
		//! top level function to compute all values requested in the assignment
		//! for each stock registered in the stock market and using the last
		//! stock price stored.
		
		//! The functions iterates over all registered stocks and calls
		//! APIs defined in stockUtil.h for the class 'Stock'.
		//! The function computes and stores the Geometric Mean at the end.
		void computeStockValues();
		
	    //! Print this stock market infos
		void printInfo() const;
		
		//! Given a stock symbol, retrieve all (including computed) stock values
		//! associated with this stock
		const Stock* findStock(const char* symbol) const;
		
		//! Return a pointer to the vector of all trades of a given symbol
		const TradesVec* getTrades(const char* symbol) const;
			
	private:
		void printTrades     () const;
		void printStockValues() const;
		
		const char* _name;
		const char* _location;
		const char* _country;
		double      _geometricMean;
		StocksMap   _stocks;
		TradesMap   _trades;
		
	//! Disable copy constructor and 
	//! copy assignment operator
	StockMarket(const StockMarket&);
	StockMarket& operator=(const StockMarket&);
};

#endif
