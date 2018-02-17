#ifndef _STOCK_UTIL_H
#define _STOCK_UTIL_H

#include <unordered_map>
#include <vector>
#include <string>
#include "time.h"

// File declares types and classes used to manage the 'Super Simple Stock Market':

class Stock;
class Trade;

typedef std::unordered_map<std::string, Stock*>    StocksMap; // maps stock symbol to associated values
typedef std::vector<Trade*>                        TradesVec; // vector of all trades of a given stock
typedef std::unordered_map<std::string, TradesVec> TradesMap; // maps stock symbol to all trades of this stock
typedef StocksMap::iterator                        StocksIter;
typedef TradesMap::iterator                        TradesMapIter;
typedef TradesVec::iterator                        TradesIter;

class Stock
{
	public:
		Stock();
		Stock(std::string const& symbol);
		Stock(const char*        symbol,
			  int                lastDividend,
			  int                parValue);
		virtual ~Stock() {}
		
		//! Accessing
		std::string const& symbol     () const { return _symbol;            }
		int         lastDividend      () const { return _lastDividend;      }
		int         parValue          () const { return _parValue;          }
		int         lastPrice         () const { return _lastPrice;         }
		double      lastDividendYield () const { return _lastDividendYield; }
		double      lastPERatio       () const { return _lastPERatio;       }
		double      weightedStockPrice() const { return _weightedStockPrice; }
			
		//! Setting
		void symbol           (std::string const& symbol) { _symbol     = symbol; }
		void symbol           (const char* symbol) { _symbol            = std::string(symbol); }
		void lastDividend     (int         value)  { _lastDividend      = value;  }
		void parValue         (int         value)  { _parValue          = value; }
		void lastPrice        (int         value)  { _lastPrice         = value;  }
		void lastDividendYield(double      value)  { _lastDividendYield = value;  }
		void lastPERatio      (double      value)  { _lastPERatio       = value;  }
		void weightedStockPrice(double      value) { _weightedStockPrice = value;  }
		
		//! Given a price, compute the dividend yield
		virtual double computeDividendYield(int price);
		
		//! Given a price, compute P/E ratio using formula Price/Divident
		// NOTE:
		// Since formula given in Assignment does not specify whether
		// 'Last Dividend' or 'Fixed Dividentd' should be used when 
		// computing P/E Ratio for a Preferred Stock, 'Last Dividend'
		// will be used for all types of stock, hence no need for virtual function
		double computePERatio(int price);
			
		// Compute the 'Volume Weighted Stock Price' for this stock trades
		double computeWeightedStockPrice(TradesVec const& trades);
		
		// Utility function to copy all data members into a new Stock
		virtual void copyData(Stock* dest) const;
				
		//! print this stock infos
		virtual void printInfo() const;
				
	private:
		std::string  _symbol;
		int          _lastDividend;
		int          _parValue;
		int          _lastPrice;
		double       _lastDividendYield;
		double       _lastPERatio;
		double       _weightedStockPrice;
};

class PreferredStock: public Stock
{
	public:
		PreferredStock();
		PreferredStock(std::string const& symbol);
		PreferredStock(const char* symbol,
					   int         lastDividend,
					   int         parValue,
					   int         fixedDividend);
		~PreferredStock() {}
		
			//! Accessing
		int fixedDividend() { return _fixedDividend; }
		
		//! Setting 
		void fixedDividend(int value) { _fixedDividend = value; }
			
		//! Given a price compute the dividend yield
		double computeDividendYield(int price);
		
		void copyData(Stock* dest) const;
		
		//! Print stock infos
		void printInfo() const;
						
	private:
		int _fixedDividend;
};

//! class to record a trade
class Trade
{
	public:	
		Trade();
		Trade(Trade const& t);
		Trade(const char* symbol,
			  int   	  price,
		      int         quantity,
			  bool        buy);
		virtual ~Trade() { }
		
		
	//! Accessing
		std::string const& symbol   () const { return _symbol;    }
		int                price    () const { return _price;     }
		int                quantity () const { return  _quantity; }
		time_t const&      timestamp() const { return _timestamp; }
		bool               buying   () const { return _buy;       }		
			
		//! Setting
		
		// NOTE:
		// Re-setting some of these values should not be allowed
		// (or restricted) to avoid unlawful manipulation
		// but these APIs are needed for testing purposes (Super simple stock market!)
		void symbol   (const char* symbol)        { _symbol    = std::string(symbol); }
		void symbol   (std::string const& symbol) { _symbol    = symbol; }
		void price    (int         value)         { _price     = value;  }
		void quantity (int         value)         { _quantity  = value;  }
		void buying   (bool        value)         { _buy       = value;  }
		void timestamp(time_t      value)         { _timestamp = value;  }
				
		//! Print this trade infos
		void printInfo() const;
	private:
		std::string _symbol;
		int         _price;
		int         _quantity;
		time_t      _timestamp;
		bool        _buy;
};

#endif
