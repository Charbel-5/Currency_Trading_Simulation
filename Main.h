#pragma once
#include "OrderBook.h"
#include "Wallet.h"

class Main{

public:

	Main();

	/** Call this method to start the simulaton */
	void init();

private:

	void printUserGuide();
	void printHelp();
	void printMarketStats();
	void enterAsk();
	void enterBid();
	void printUserOrders();
	void printWallet();
	void gotoNextTimeframe();
	int getUserOption();
	void processUserOption(int userOption);

	std::string currentTime;

	OrderBook orderBook{"test_data.csv"};

	Wallet wallet;

};