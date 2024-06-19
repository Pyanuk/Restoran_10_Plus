#include <string>
#include "JsonHelper.h"
#include <windows.h>
#include "Menu.h"
#include "Order.h"
#include "Guest.h"

using namespace std;

struct MenuItem {
    std::string name;
    double cost;
};

void Guest::displayCart() {
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");


    if (cartData["cart"].empty()) {
        cout << "Корзина пуста." << endl;
        Sleep(1500);
        system("cls");
        return;
    }


    cout << "Содержимое корзины:\n";
    bool foundOrders = false;
    for (const auto& item : cartData["cart"]) {
        if (item["status"] == "In processing") {
            cout << "Название: " << item["name"].get<string>() << endl;
            cout << "Цена: " << item["cost"].get<double>() << " рублей" << endl;
            cout << "--------------------------\n";
            foundOrders = true;
        }
    }

    if (!foundOrders) {
        cout << "Корзина пуста." << endl;
    }

    int choice;
    std::cout << "Введите любой символ для выхода: ";
    std::cin >> choice;
    system("cls");
}

void Guest::createOrder() {
    JsonHelper jsonHelper;
    system("cls");


    json menuData = jsonHelper.readJsonData("menu.json");
    json productsData = jsonHelper.readJsonData("products.json");
    json guestData = jsonHelper.readJsonData("guestOrder.json");

    if (menuData["menu"].empty()) {
        cout << "Меню пусто." << endl;
        Sleep(1500);
        system("cls");
        return;
    }


    bool isAvailable = true;
    cout << "Текущее меню:\n";
    for (size_t i = 0; i < menuData["menu"].size(); ++i) {
        cout << "Запись " << i + 1 << ":\n";
        cout << "Название: " << menuData["menu"][i]["name"].get<string>() << endl;
        cout << "Описание: " << menuData["menu"][i]["description"].get<string>() << endl;
        cout << "Время приготовления: " << menuData["menu"][i]["cookingTime"].get<string>() << " минут" << endl;
        cout << "Цена: " << menuData["menu"][i]["cost"].get<double>() << " рублей" << endl;
        cout << "Вес блюда: " << menuData["menu"][i]["weight"].get<string>() << " грамм" << endl;
        cout << "Ингридиенты: " << endl;
        for (const auto& ingredient : menuData["menu"][i]["ingredients"]) {
            cout << "   - " << ingredient["name"].get<string>() << endl;
        }
        cout << "--------------------------\n";
    }

    int recordNumber;
    cout << "Введите номер блюда для выбора: ";
    cin >> recordNumber;

    if (recordNumber <= 0 || recordNumber > menuData["menu"].size()) {
        cout << "Введен некорректный номер записи." << endl;
        system("pause");
        system("cls");
        return;
    }

    
    bool ingredientsAvailable = true;


    std::unordered_map<std::string, int> productQuantities;

    for (const auto& product : productsData["products"]) {
        std::string productName = product["name"].get<std::string>();

        if (productQuantities.find(productName) != productQuantities.end()) {

            productQuantities[productName]++;
        }
        else {

            productQuantities[productName] = 1;
        }
    }

    for (const auto& ingredient : menuData["menu"][recordNumber - 1]["ingredients"]) {
        std::string ingredientName = ingredient["name"].get<std::string>();
        int requiredQuantity = 1; 

        if (productQuantities.find(ingredientName) == productQuantities.end() ||
            productQuantities[ingredientName] < requiredQuantity) {

            ingredientsAvailable = false;
            break;
        }
    }

    if (ingredientsAvailable) {

        MenuItem selectedMenuItem;


        selectedMenuItem.name = menuData["menu"][recordNumber - 1]["name"].get<std::string>();
        selectedMenuItem.cost = menuData["menu"][recordNumber - 1]["cost"].get<double>();



        cout << endl << "Блюдо добавлено в корзину.";

        json cartData = jsonHelper.readJsonData("cart.json");

        cartData["cart"].push_back({
            {"name", selectedMenuItem.name},
            {"cost", selectedMenuItem.cost},
            {"status", "In processing"}
            });

        jsonHelper.writeJsonData("cart.json", cartData);

        Sleep(1500);
        system("cls");
    }
    else {
        system("cls");
        cout << "Недостаточно ингредиентов. Выбранное блюдо нельзя добавить в заказ." << endl;
        Sleep(1500);
        system("cls");
    }

}

void Guest::confirmOrder() {
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");


    if (cartData["cart"].empty()) {
        cout << "Корзина пуста." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    cout << "Корзина:\n";
    double orderTotal = 0.0;
    bool foundOrders = false;
    for (const auto& order : cartData["cart"]) {
        if (order["status"] == "In processing") {
            cout << "Блюдо: " << order["name"].get<string>() << endl;
            cout << "Цена: " << order["cost"].get<double>() << " рублей" << endl;
            orderTotal += order["cost"].get<double>();
            cout << "--------------------------\n";
            foundOrders = true;
        }
    }

    if (!foundOrders) {
        cout << "Нет заказов для оплаты." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    cout << "Общая стоимость заказа: " << orderTotal << " рублей" << endl;

    double totalAmount;
    cout << "Введите сумму денег: ";
    cin >> totalAmount;

    if (totalAmount < orderTotal) {
        cout << "Введенная сумма меньше стоимости заказа. Иди работай." << endl;
        Sleep(1500);
        system("cls");
        return;
    }


    double restaurantBalance = 0.0;
    ifstream balanceFile("RestBalance.txt");
    if (balanceFile.is_open()) {
        balanceFile >> restaurantBalance;
        balanceFile.close();
    }

    restaurantBalance += orderTotal;
    ofstream updatedBalanceFile("RestBalance.txt");
    if (updatedBalanceFile.is_open()) {
        updatedBalanceFile << restaurantBalance;
        updatedBalanceFile.close();
    }

    json selledMenuData;


    ifstream selledMenuFile("selledMenu.json");
    if (selledMenuFile.is_open()) {
        selledMenuFile >> selledMenuData;
        selledMenuFile.close();
    }



    for (auto& order : cartData["cart"]) {
        if (order["status"] == "In processing") {
            order["status"] = "Paid for and transferred to the kitchen";

            selledMenuData["selledMenu"].push_back(order);
        }
    }


    ofstream updatedSelledMenuFile("selledMenu.json");
    if (updatedSelledMenuFile.is_open()) {
        updatedSelledMenuFile << selledMenuData;
        updatedSelledMenuFile.close();
    }


    jsonHelper.writeJsonData("cart.json", cartData);

    cout << "Заказ подтвержден и передан на кухню." << endl;
    Sleep(1500);
    system("cls");
}

void Guest::displayStatus(){
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");

    if (cartData["cart"].empty()) {
        cout << "Корзина пуста." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    cout << "Статус заказов:\n";
    for (const auto& order : cartData["cart"]) {
        cout << "Блюдо: " << order["name"].get<string>() << endl;
        cout << "Статус: " << order["status"].get<string>() << endl;
        cout << "--------------------------\n";
    }

    system("pause");
    system("cls");
}

