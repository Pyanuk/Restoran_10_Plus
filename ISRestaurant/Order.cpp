#include "Order.h"
#include "JsonHelper.h"
#include <ctime>
#include <set>
#include <string>
#include <windows.h>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

double getBalance() {
    std::ifstream balanceFile("RestBalance.txt");
    if (!balanceFile) {

        std::cerr << "Ошибка открытия файла RestBalance.txt." << std::endl;
        return 0.0; 
    }

    double balance;
    if (!(balanceFile >> balance)) {

        std::cerr << "Ошибка чтения баланса из файла RestBalance.txt." << std::endl;
        return 0.0; 
    }

    balanceFile.close();
    return balance;
}

void Order::createOrder() {
    JsonHelper jsonHelper;
    system("cls");

    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &currentTime);
    char timeBuffer[26];
    asctime_s(timeBuffer, sizeof(timeBuffer), &localTime);
    std::string sendTime(timeBuffer);

    json productData = jsonHelper.readJsonData("products.json");


    std::cout << "Список продуктов:\n";
    std::set<int> printedIds; 
    for (const auto& product : productData["products"]) {
        int productId = product["id"].get<int>();
        if (printedIds.find(productId) == printedIds.end()) {
          
            printedIds.insert(productId);
            std::cout << "ID: " << productId << std::endl;
            std::cout << "Наименование: " << product["name"] << std::endl;
            std::cout << "Цена: " << product["price"] << std::endl;
            std::cout << "-----------------------\n";
        }
    }

    int productId;
    int quantity;

    std::cout << "Введите ID продукта: ";
    std::cin >> productId;

    std::cout << "Введите количество: ";
    std::cin >> quantity;


    json selectedProduct = jsonHelper.getProductData(productId);


    if (selectedProduct.empty()) {
        std::cout << "Продукт с указанным ID не найден." << std::endl;
        system("pause");
        system("cls");
        return;
    }

    double totalPrice = selectedProduct["price"].get<double>() * quantity;

    double balance = getBalance();


    if (balance < totalPrice) {
        std::cout << "Недостаточно средств на балансе ресторана." << std::endl;
        system("pause");
        system("cls");
        return;
    }

    json orderData = jsonHelper.readJsonData("orders.json");

    json newOrder;
    newOrder["product_id"] = productId;
    newOrder["product_name"] = selectedProduct["name"];
    newOrder["quantity"] = quantity;
    newOrder["send_time"] = sendTime;


    orderData["orders"].push_back(newOrder);


    jsonHelper.writeJsonData("orders.json", orderData);


    json sendOrdersData = jsonHelper.readJsonData("SendOrders.json");
    sendOrdersData["SendOrders"].push_back(newOrder);
    jsonHelper.writeJsonData("SendOrders.json", sendOrdersData);

    std::cout << "Заявка успешно создана." << std::endl;

    system("pause");
    system("cls");
}

void Order::withdrawFunds(double amount){
    double currentBalance = getBalance();

    if (currentBalance < amount) {
        std::cout << "Недостаточно средств на балансе ресторана." << std::endl;
        return;
    }


    double updatedBalance = currentBalance - amount;


    std::ofstream balanceFile("RestBalance.txt");
    if (balanceFile.is_open()) {
        balanceFile << updatedBalance;
        balanceFile.close();
  
    }
    else {
        std::cout << "Ошибка при обновлении баланса ресторана." << std::endl;
    }
}

void Order::printSendOrders() {
    system("cls");
    JsonHelper jsonHelper;

    json sendOrderData = jsonHelper.readJsonData("SendOrders.json");

    if (sendOrderData["SendOrders"].empty()) {
        std::cout << "Список отправленных заявок пуст." << std::endl;
        return;
    }


    std::cout << "Список отправленных заявок:" << std::endl;
    for (const auto& sendOrder : sendOrderData["SendOrders"]) {
        std::cout << "Название продукта: " << sendOrder["product_name"] << std::endl;
        std::cout << "Количество: " << sendOrder["quantity"] << std::endl;
        std::cout << "Время отправки: " << sendOrder["send_time"] << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    int choice;
    std::cout << "Введите любой символ для выхода: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
    }
}

void Order::ordersList() {
    JsonHelper jsonHelper;
    system("cls");


    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &currentTime);
    char timeBuffer[26];
    asctime_s(timeBuffer, sizeof(timeBuffer), &localTime);
    std::string sendTime(timeBuffer);



    json orderData = jsonHelper.readJsonData("orders.json");
    json productsData = jsonHelper.readJsonData("products.json");
    json existingData = jsonHelper.readJsonData("DoneOrders.json");


    if (orderData["orders"].empty()) {
        cout << "Список заявок пуст." << endl;
        Sleep(1500);
        system("cls");
        return;
    }



    const int itemsPerPage = 5;
    int totalPages = (orderData["orders"].size() + itemsPerPage - 1) / itemsPerPage; 

    std::cout << "Список заявок:\n";
    int startIndex = 0;
    int currentPage = 1;
    while (true) {
        int endIndex = startIndex + itemsPerPage;
        if (endIndex > orderData["orders"].size()) {
            endIndex = orderData["orders"].size();
        }

        for (int i = startIndex; i < endIndex; ++i) {
            int productId = orderData["orders"][i]["product_id"].get<int>();
            int quantityPro = orderData["orders"][i]["quantity"].get<int>();

            std::cout << "Наименование: " << orderData["orders"][i]["product_name"] << std::endl;
            std::cout << "Количество: " << quantityPro << std::endl;
            std::cout << "-----------------------\n";
        }

        bool isLastPage = currentPage == totalPages;


        if (isLastPage) {
            std::cout << "Это последняя страница. Нет доступных записей для загрузки.\n";
            break;
        }


        std::cout << "Введите 'y' для загрузки следующей страницы, или любой другой символ для выхода: ";
        std::string input;
        std::cin >> input;

        if (input != "y") {
            break;
        }


        startIndex += itemsPerPage;
        currentPage++;
    }

    string choice;
    double price = 0;
    int quantity = 0;
    double amount = 0;

    cout << "Введите название продукта для отправки: ";
    cin >> choice;


    for (const auto& product : productsData["products"]) {
        if (product["name"] == choice) {
            price = product["price"].get<std::double_t>();
            break;
        }
        else {
            system("cls");
            cout << "Неверное имя продукта!\n";
            Sleep(1500);
            return;
        }
    }

    for (const auto& order : orderData["orders"]) {
        if (order["product_name"] == choice) {
            quantity = order["quantity"].get<std::int16_t>();
            break;
        }
        else {
            system("cls");
            cout << "Неверное имя продукта!\n";
            Sleep(1500);
            return;
        }
    }

    amount = price * quantity;


    int Id = 0;

    for (const auto& product : productsData["products"]) {
        if (product["name"] == choice) {
            Id = product["id"].get<int>();
        }
    }

    if (choice.empty()) {
        cout << "Заявка с указанным ID не найдена." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    withdrawFunds(amount);
    for (int i = 0; i < quantity; ++i) {
        productsData["products"].push_back({
            {"id", Id},
            {"name", choice},
            {"price", price}
            });
    }

    jsonHelper.writeJsonData("products.json", productsData);

    json purchasedData = jsonHelper.readJsonData("purchasedProducts.json");
    purchasedData["purchasedProducts"].push_back({
        {"name", choice},
        {"amount", amount}
        });
    jsonHelper.writeJsonData("purchasedProducts.json", purchasedData);


    json updatedOrderData;
    for (const auto& order : orderData["orders"]) {
        if (!(order["product_name"] == choice && order["quantity"].get<int>() == quantity)) {
            updatedOrderData.push_back(order);
        }
        else {

            json doneOrderData = {
                {"product_id", order["product_id"]},
                {"product_name", order["product_name"]},
                {"quantity", order["quantity"]},
                {"date_executed", sendTime} 
            };


            existingData["DoneOrders"].push_back(doneOrderData);
            jsonHelper.writeJsonData("DoneOrders.json", existingData);
        }
    }


    orderData["orders"] = updatedOrderData;


    jsonHelper.writeJsonData("orders.json", orderData);

    cout << "Заявка успешно обработана!" << endl;

    system("pause");
    system("cls");
}

void Order::printAcceptOrders() {
    system("cls");
    JsonHelper jsonHelper;



    json doneOrderData = jsonHelper.readJsonData("DoneOrders.json");


    if (doneOrderData["DoneOrders"].empty()) {
        std::cout << "Список принятых продуктов пуст." << std::endl;
        return;
    }


    std::cout << "Список принятых продуктов:" << std::endl;
    for (const auto& doneOrder : doneOrderData["DoneOrders"]) {
        cout << "Дата: " << doneOrder["date_executed"] << endl;
        cout << "ID продукта: " << doneOrder["product_id"] << endl;
        cout << "Название продукта: " << doneOrder["product_name"] << endl;
        std::cout << "Количество: " << doneOrder["quantity"] << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    int choice;
    std::cout << "Введите любой символ для выхода: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
    }
}