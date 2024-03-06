#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook{

public:

	/**  Constructor for reading a csv data file */
	OrderBook(std::string filename);

	/** Returns a vector of all known products in the dataset */
	std::vector<std::string> getKnownProducts();

	/** Returns vector of orders according to sent filters */
	std::vector<OrderBookEntry> getOrders(
		OrderBookType type,
		std::string product,
		std::string timestamp);

	/** Returns orders placed by the user */
	void getUserPlacedOrders();

	void displayMenu();

	/** Returns the earliest time in the OrderBook */
	std::string getEarliestTime();

	/** Returns the next time after the sent time in the OrderBook. If there is no next timestamp, wraps around to the start.*/
	std::string getNextTime(std::string timestamp);

	void insertOrder(OrderBookEntry& order);

	/** Matches asks to bids */
	std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

	static double getHighPrice(std::vector<OrderBookEntry>& orders);

	static double getLowPrice(std::vector<OrderBookEntry>& orders);

	/** Deletes unrealized user orders after each timestamp */
	void deleteUserOrdersFromOrderBook();

private:

	std::vector<OrderBookEntry> orders;

};