#include "Wallet.h"
#include <iostream>
#include "CSVReader.h"



Wallet::Wallet(){

}



void Wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    if (amount < 0)
    {
        throw std::exception{};
    }
    if (currencies.count(type) == 0) // Not in wallet yet
    {
        balance = 0;
    }
    else
    {
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}



bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        return false;
    }
    if (currencies.count(type) == 0) // Not in wallet yet
    {
        return false;
    }
    else
    {
        if (containsCurrency(type, amount)) // Checks if user has enough currency
        {
            currencies[type] -= amount;
            return true;
        }
        else // User does not have enough
            return false;
    }
}



bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0) // User doesn't have that currency
        return false;
    else
        return currencies[type] >= amount;
}



bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');

    // Ask
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << std::endl;
        std::cout << "You want to sell " << currency << ". " << amount << " " << currency << " will be deduced if matched. You can always check and cancel orders from \"Check your Orders\" menu." << std::endl << std::endl;
        return containsCurrency(currency, amount);
    }

    // Bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << std::endl;
        std::cout << "You want to sell " << currency << ". " << amount << " " << currency << " will be deduced if matched. You can always check and cancel orders from \"Check your Orders\" menu." << std::endl << std::endl;
        return containsCurrency(currency, amount);
    }
    return false;
}



void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

    // If ask
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    // If bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}



std::string Wallet::toString() // Print the currencies and amounts present in the wallet by building a string out of them
{
    std::string s;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    if (s.empty()) {
        std::cout << "Your wallet is empty" << std::endl;
    }
    return s;
}