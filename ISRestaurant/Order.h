#pragma once

#include <vector>
#include <string>

class Order {
public:
    void createOrder();
    void withdrawFunds(double amount);
    void printSendOrders();
    void ordersList();
    void printAcceptOrders();
};
