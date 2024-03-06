#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include<iomanip>



OrderBook::OrderBook(std::string filename)
{
	orders = CSVReader::readCSV(filename);
};



std::vector<std::string> OrderBook::getKnownProducts()
{
	std::vector<std::string> products;
	std::map<std::string, bool> prodMap;

	for (OrderBookEntry& e : orders)
	{
		prodMap[e.product] = true;
	}

	// Flattening the map to a vector of strings
	for (auto const& e : prodMap)
	{
		products.push_back(e.first);
	}

	return products;
};



std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp) 
{
	std::vector<OrderBookEntry> orders_sub;

	for (OrderBookEntry& e : orders)
	{
		if (e.orderType == type && e.product == product && e.timestamp == timestamp)
		{
			orders_sub.push_back(e);
		}
	}

	return orders_sub;
}



void OrderBook::getUserPlacedOrders() {
	std::cout << std::endl;
	std::cout << "Your Orders So Far:" << std::endl << std::endl;
	std::cout << std::left << std::setw(8) << "Index" << std::setw(7) << "Type" << std::setw(10) << "Product" << std::setw(12) << "Price" << std::setw(10) << "Amount" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	// Stores indices of user orders for deletion
	std::vector<int> userOrderIndices;

	std::string type;
	int count = 0, userInput, orderIndex = 0;
	for (int i = 0; i < orders.size(); ++i) {
		OrderBookEntry& e = orders[i];
		if (e.username == "simuser") {
			count++;
			userOrderIndices.push_back(i); // Stores index for deletion

			if (e.orderType == OrderBookType::ask) type = "ask";
			if (e.orderType == OrderBookType::bid) type = "bid";

			std::cout << std::left << std::setw(8) << count << std::setw(7) << type << std::setw(10) << e.product << std::setw(12) << e.price << std::setw(10) << e.amount << std::endl;
		}
	}
	if (count == 0) {
		std::cout << "       There are Curenntly No Orders" << std::endl;
		std::cout << "-------------------------------------------" << std::endl << std::endl;
		return;
	}
	else std::cout << "-------------------------------------------" << std::endl << std::endl;

	while (true) {

		displayMenu();
		std::cout << "What would you like to do?: ";
		std::cin >> userInput;

		while (userInput != 1 && userInput != 2 && userInput !=3) {
			std::cout << "Choose either 1-3: ";
			std::cin >> userInput;
		}

		if (userInput == 1) {
			if (count > 0) {
				std::cout << "Which order would you like to cancel? (Use Indices): ";
				std::cin >> orderIndex;
				if (orderIndex > 0 && orderIndex <= userOrderIndices.size()) {
					int indexToDelete = userOrderIndices[orderIndex - 1];
					orders.erase(orders.begin() + indexToDelete); // Delete order from orders vector
					std::cout << std::endl << "Order canceled successfully" << std::endl;
				}
				else {
					std::cout << "Invalid order index" << std::endl;
				}
			}
			else {
				std::cout << "There are no orders to cancel." << std::endl;
			}	
		}

		if (userInput == 2) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getUserPlacedOrders();
			return;
		}

		if (userInput == 3) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return;
		}
	}
}



void OrderBook::displayMenu() {
	std::cout << std::endl;
	std::cout << "Menu" << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "1: Cancel an order" << std::endl;
	std::cout << "2: Check Orders Again" << std::endl;
	std::cout << "3: Get Back to Trading!" << std::endl;
	std::cout << "-----------------------" << std::endl;
}



// Assumes that orders are sorted
std::string OrderBook::getEarliestTime()
{
	return orders[0].timestamp;
}



// Assumes that orders are sorted by timestamps 
std::string OrderBook::getNextTime(std::string timestamp)
{
	std::string next_timestamp = "";
	for (OrderBookEntry& e : orders)
	{
		if (e.timestamp > timestamp)
		{
			next_timestamp = e.timestamp;
			break;
		}
	}

	// To loop through the order book by returning to the first timestamp after going throught all of it
	if (next_timestamp == "")
	{
		next_timestamp = orders[0].timestamp;
	}
	return next_timestamp;
}



void OrderBook::insertOrder(OrderBookEntry& order)
{
	orders.push_back(order);
	std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}



std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
		product,
		timestamp);
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
		product,
		timestamp);

	std::vector<OrderBookEntry> sales;

	if (asks.size() == 0 || bids.size() == 0)
	{
		std::cout << "No bids or asks found" << std::endl;
		return sales;
	}

	// Sort asks by lowest first
	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

	// Sort bids by highest first
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

	std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
	std::cout << "min ask " << asks[0].price << std::endl;
	std::cout << "max bid " << bids[0].price << std::endl;
	std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;


	for (OrderBookEntry& ask : asks)
	{
		for (OrderBookEntry& bid : bids)
		{
			if (bid.price >= ask.price)
			{

				OrderBookEntry sale{ ask.price, 0, timestamp, product, OrderBookType::asksale };

				if (bid.username == "simuser")
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::bidsale;
				}
				if (ask.username == "simuser")
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::asksale;
				};
			
				if (bid.amount == ask.amount)

				{
					sale.amount = ask.amount;
					sales.push_back(sale);
					bid.amount = 0;
					break;
				}

				if (bid.amount > ask.amount)
				{
					sale.amount = ask.amount;
					sales.push_back(sale);
					bid.amount = bid.amount - ask.amount;
					break;
				}

				if (bid.amount < ask.amount && bid.amount > 0)
				{
					sale.amount = bid.amount;
					sales.push_back(sale);
					ask.amount = ask.amount - bid.amount;
					bid.amount = 0;
					continue;
				}
			}
		}
	}
	return sales;
}



double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
	double max = orders[0].price;
	for (OrderBookEntry& e : orders)
	{
		if (e.price > max) max = e.price;
	}
	return max;
}



double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
	double min = orders[0].price;
	for (OrderBookEntry& e : orders)
	{
		if (e.price < min) min = e.price;
	}
	return min;
}



void OrderBook::deleteUserOrdersFromOrderBook() {
	for (auto it = orders.begin(); it != orders.end(); ) {
		if (it->username == "simuser") {
			it = orders.erase(it);
		}
		else {
			++it;
		}
	}
}