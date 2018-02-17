#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H

// Utility functions for testing stock market functionality

// Forward declaration
class StockMarket;
class Tester
{
	public:
		void addStockData       (StockMarket& stockMarket);
		void check              (StockMarket& stockMarket);
		
	private:
	//! For each given stock symbol, check if stock object exists and
	//! check these values: Last dividend,  Par Value, Last price,
    //! Dividend Yield, P/E Ratio and Volume Weighted Stock Price
    //! Returns True if all actual function parameters match expected values
		bool checkStock(const char*  symbol,
						int          lastDividend,
						int          parValue,
						int          lastPrice,
						double       dividendYield,
						double       PERatio,
						double       vwap,
						StockMarket& stockMarket);
};

#endif
