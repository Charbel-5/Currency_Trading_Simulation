#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet{

public:

	/** Wallet constructor */
	Wallet();

	/** Innsert currency to the wallet */
	void insertCurrency(std::string type, double amount);

	/** Remove currency from the wallet */
	bool removeCurrency(std::string type, double amount);

	/** Checks if the wallet contains a certain amount of currency */
	bool containsCurrency(std::string type, double amount);

	/** Checks if the wallet can cope with this ask or bid */
	bool canFulfillOrder(OrderBookEntry order);

	/** Updates the contents of the wallet. Assumes the order was made by the owner of the wallet */
	void processSale(OrderBookEntry& sale);

	/** Generates a string representation of the wallet */
	std::string toString();

private:

	std::map<std::string, double> currencies;

};