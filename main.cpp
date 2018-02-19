#include <iostream>
#include <iomanip>
#include "stockMarket.h"
#include "stockUtil.h"
#include "testUtil.h"

using namespace std;

int main(int argc, char** argv) {

	std::cout << std::setprecision(2) << std::fixed;
	
	cout << "\n\tASSIGNMENT - SUPER SIMPLE STOCK MARKET" << endl;
	cout << "\t--------------------------------------\n" << endl;
	
	StockMarket london   ("FTSE100", "London", "UK");
	StockMarket frankfurt("DAX", "Frankfurt", "Germany");
	
	Tester test;
	test.addStockData        (london);
	london.computeStockValues();
	london.printInfo         ();
	test.check               (london);
	cout << endl << endl;

	// Test other stock market with empty values
	// Should not crash!
	frankfurt.computeStockValues();	
	frankfurt.printInfo         ();
	
	return 0;
}
