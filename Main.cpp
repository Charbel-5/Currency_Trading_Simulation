#include<iostream>
#include "Main.h"
#include <vector>
#include "CSVReader.h"
#include "OrderBook.h"



Main::Main()
{

}



void Main::init() {

	int input;
	currentTime = orderBook.getEarliestTime();

	wallet.insertCurrency("BTC", 10);
	wallet.insertCurrency("ETH", 5);

	while (true) {
		printUserGuide();
		input = getUserOption();
		processUserOption(input);
	}
}



void Main::printUserGuide()
{
	std::cout << "=========================" << std::endl;

	// 1 print user guide
	std::cout << "1: Print User Guide" << std::endl;
	// 2 print exchange stats
	std::cout << "2: Print Exchange Stats" << std::endl;
	// 3 make an offer
	std::cout << "3: Make an Ask (Sell)" << std::endl;
	// 4 make a bid
	std::cout << "4: Make a Bid (Buy)" << std::endl;
	// 5 check user orders so far
	std::cout << "5: Check your Orders" << std::endl;
	// 6 print wallet
	std::cout << "6: Print Wallet" << std::endl;
	// 7 continue
	std::cout << "7: Continue" << std::endl;
	// 8: terminate simulation
	std::cout << "8: Terminate Simulation" << std::endl;

	std::cout << "=========================" << std::endl;

	std::cout << "Current time is: " << currentTime << std::endl;
}



void Main::printHelp() {

	// General Information About the simulation
	std::cout << "General Information" << std::endl << "-------------------" << std::endl;
	std::cout << "This simulation aims solely to enhance my object-oriented programming skills within a course I enrolled in. It does not mirror the actual processes of trading in the real world." << std::endl << "Whenever the simulation loads users get 10 BTC (Bitcoin) and 5 ETH (Ethereum) to practice their trading skills." << std::endl << std::endl << std::endl;

	// Option "Print Exchange Stats" explanation
	std::cout << "Option \"Print Exchange Stats\"" << std::endl << "-----------------------------" << std::endl;
	std::cout << "Displays the minimum and maximum asks available in the market." << std::endl << "This provides users with insight into market prices." << std::endl << std::endl << std::endl;

	// Option "Make an Ask (Sell)" explanation
	std::cout << "Option \"Make an Ask (Sell)\"" << std::endl << "---------------------------" << std::endl;
	std::cout << "Allows users to place an ask specifying the currency they want to trade (e.g., BTC), the currency they want to trade it with (e.g., USDT), the price they are expecting to receive in return for a single unit traded (e.g., how much USDT they want in return for one BTC, e.g., 3000), and the quantity of the currency they want to sell (e.g., how much BTC they want to trade, e.g., 0.7)." << std::endl;
	std::cout << std::endl << "Example Asks:" << std::endl << "BTC/USDT,5284.31,0.01" << std::endl << "ETH/BTC,0.0213,0.4" << std::endl << "BTC/USDT,3000,0.7" << std::endl << std::endl << std::endl << std::endl;

	// Option "Make a Bid (Buy)" explanation
	std::cout << "Option \"Make a Bid (Buy)\"" << std::endl << "-------------------------" << std::endl;
	std::cout << "Enables users to place a bid specifying the currency they want to trade for (e.g., USDT), the currency they want to trade with (e.g., BTC), the price they are willing to pay for a single unit of the desired currency (e.g., how much BTC they are willing to trade for one USDT, e.g., 0.001), and the quantity of the desired currency they want to buy (e.g., the amount of USDT they want to purchase, e.g., 500)." << std::endl;
	std::cout << std::endl << "Example Bids:" << std::endl << "DOGE/BTC,0.000001,200" << std::endl << "ETH/BTC,0.7,1.6" << std::endl << "DOGE/BTC,0.000001,450" << std::endl << std::endl << std::endl << std::endl;

	// Option "Check your Orders"
	std::cout << "Option \"Check your Orders\"" << std::endl << "--------------------------" << std::endl;
	std::cout << "Allows users to view their pending orders and gives them the ability to delete any unrealized pending asks and bids in the current timeframe." << std::endl << std::endl << std::endl;

	// Option "Print Wallet"
	std::cout << "Option \"Print Wallet\"" << std::endl << "---------------------" << std::endl;
	std::cout << "Displays the currencies and corresponding amounts held by the user in their wallet." << std::endl << std::endl << std::endl;

	// Option "Continue"
	std::cout << "Option \"Continue\"" << std::endl << "-----------------" << std::endl;
	std::cout << "Advances the simulated time within the app. During this progression, new market asks and bids are generated, and the simulation attempts to match them with the user's asks and bids." << std::endl;
	std::cout << "After selecting this option, the app displays whether the user's placed asks and bids were matched, along with the corresponding amounts and prices." << std::endl;
	std::cout << "This allows users to check if they made any profit, all within the context of the simulated environment where time progresses independently of real-world time." << std::endl;
	std::cout<< "!! Note that all non realized asks and bids from the previous TimeStamp will be deleted !!" << std::endl << std::endl << std::endl;
	
	// Option "Terminate Simulation"
	std::cout << "Option \"Terminate Simulation\"" << std::endl << "-----------------------------" << std::endl;
	std::cout << "Ends the simulation" << std::endl << std::endl << std::endl;

}



void Main::printMarketStats() {
	for (std::string const& p : orderBook.getKnownProducts()) {
		std::cout << "---------------" << std::endl;
		std::cout << "Product: " << p << std::endl;
		std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
		std::cout << "Asks seen: " << entries.size() << std::endl;
		std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
		std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
		std::cout << std::endl;
	}
}



void Main::enterAsk() {
	std::cout << "Make an Ask (Sell) - enter the amount in the format: product,price,amount (e.g.: BTC/USDT,5284.31,0.01) " << std::endl;
	std::string input;
	getline(std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

	if (tokens.size() != 3)
	{
		std::cout << "Entered Ask is a Bad input! " << input << std::endl;
	}
	else
	{
		try
		{
			OrderBookEntry obe = CSVReader::stringsToOBE(
				tokens[1],
				tokens[2],
				currentTime,
				tokens[0],
				OrderBookType::ask);

			obe.username = "simuser";

			if (wallet.canFulfillOrder(obe))
			{
				std::cout << "Good News! You have enough money to place this order." << std::endl << std::endl;
				orderBook.insertOrder(obe);
			}
			else
			{
				std::cout << "Wallet has insufficient funds!!" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Entered Ask is a Bad input! " << std::endl;
		}
	};
}



void Main::enterBid() {
	std::cout << "Make a Bid (Buy) - enter the amount in the format: product,price,amount (e.g.: ETH/BTC,0.7,1.6)" << std::endl;
	std::string input;
	std::getline(std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

	if (tokens.size() != 3)
	{
		std::cout << "Entered Bid is a Bad input! " << input << std::endl;
	}
	else
	{
		try
		{
			OrderBookEntry obe = CSVReader::stringsToOBE(
				tokens[1],
				tokens[2],
				currentTime,
				tokens[0],
				OrderBookType::bid);

			obe.username = "simuser";

			if (wallet.canFulfillOrder(obe))
			{
				std::cout << "Good News! You have enough money to place this order." << std::endl << std::endl;
				orderBook.insertOrder(obe);
			}
			else
			{
				std::cout << "Wallet has insufficient funds!!" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Entered Bid is a Bad input " << std::endl;
		}
	};
}



void Main::printUserOrders() {
	orderBook.getUserPlacedOrders();
}



void Main::printWallet() {
	std::cout << "You have:" << std::endl;
	std::cout << "---------" << std::endl;
	std::cout << wallet.toString() << std::endl;
}



void Main::gotoNextTimeframe() {

	std::cout << "Previous Timeframe's Market Stats:" << std::endl << "----------------------------------" << std::endl;
	
	for (std::string p : orderBook.getKnownProducts())
	{
		std::cout << "--------------------" << std::endl;
		std::cout << "matching " << p << std::endl;
		std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
		std::cout << "Sales: " << sales.size() << std::endl;
		for (OrderBookEntry& sale : sales)
		{
			std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
			if (sale.username == "simuser")
			{
				// Update the wallet after the sales
				wallet.processSale(sale);
			}
		}
		std::cout << "--------------------" << std::endl;
	}

	orderBook.deleteUserOrdersFromOrderBook();
	std::cout << std::endl << "All unrealised orders (if any) were deleted" << std::endl << std::endl;
	std::cout << "Going to next time frame... " << std::endl;
	
	currentTime = orderBook.getNextTime(currentTime);
	std::cout << "#############################################" << std::endl;
}



int Main::getUserOption()
{
	int userOption = 0;
	std::string line;
	std::cout << "Type 1-8: ";
	std::getline(std::cin, line);
	try
	{
		userOption = std::stoi(line);
	}
	catch (const std::exception& e)
	{
		// Catch exception to avoid crashing
	}
	return userOption;
}



void Main::processUserOption(int userOption) {

	system("CLS"); // To clear the console

	if (userOption == 1) {
		printHelp();
	}
	else if (userOption == 2) {
		printMarketStats();
	}
	else if (userOption == 3) {
		enterAsk();
	}
	else if (userOption == 4) {
		enterBid();
	}
	else if(userOption == 5){
		printUserOrders();
	}
	else if (userOption == 6) {
		printWallet();
	}
	else if (userOption == 7) {
		gotoNextTimeframe();
	}
	else if (userOption == 8) {
		exit(0);
	}
	else {
		std::cout << "Invalid choice." << std::endl;
	}
}