#include "JsonHelper.h"
#include "Product.h"
#include "Menu.h"
#include "User.h"
#include "ISRestaurant.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#define NOMINMAX
#include <windows.h>
#include "Order.h"
#include "Guest.h"

using namespace std;


void User::editingEmployees() {
    system("cls");

    int choice;

    while (true) {
        cout << "�������������� ������� �������:\n";
        cout << "1. ������ ������� �������\n";
        cout << "2. ��������\n";
        cout << "3. ��������\n";
        cout << "4. �������\n";
        cout << "5. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            listEmployees();
            break;
        case 2:
            addEmployees();
            break;
        case 3:
            editEmployees();
            break;
        case 4:
            removeEmployees();
            break;
        case 5:
            system("cls");
            return;  
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            break;
        }
    }
}
void User::addEmployees()
{
    JsonHelper jsonHelper;
    system("cls");


    json jsonData = jsonHelper.readJsonData("users.json");

    std::string firstName, lastName, patronymic, username, password, role;

    std::cout << "���������� ������ ����������:\n";

    std::cout << "���: ";
    std::cin.ignore();
    std::getline(std::cin, firstName);

    std::cout << "�������: ";
    std::getline(std::cin, lastName);

    std::cout << "��������: ";
    std::getline(std::cin, patronymic);

    std::cout << "�����: ";
    std::getline(std::cin, username);

    std::cout << "������: ";
    std::getline(std::cin, password);

    string newHashedPass = hashPassword(password);

    std::cout << "����: ";
    std::getline(std::cin, role);

    json newEmployee;
    newEmployee["firstName"] = firstName;
    newEmployee["lastName"] = lastName;
    newEmployee["patronymic"] = patronymic;
    newEmployee["username"] = username;
    newEmployee["password"] = newHashedPass;
    newEmployee["role"] = role;

    jsonData["users"].push_back(newEmployee);

    jsonHelper.writeJsonData("users.json", jsonData);

    std::cout << "��������� ������� ��������.\n";
    Sleep(1500);
    system("cls");
}
void User::editEmployees()
{
    JsonHelper jsonHelper;
    
    system("cls");

    json jsonData = jsonHelper.readJsonData("users.json");

    std::cout << "������ �����������:\n";
    for (const auto& user : jsonData["users"]) {
        std::cout << "���: " << user["firstName"] << std::endl;
        std::cout << "�������: " << user["lastName"] << std::endl;
        std::cout << "��������: " << user["patronymic"] << std::endl;
        std::cout << "�����: " << user["username"] << std::endl;
        std::cout << "������: " << user["password"] << std::endl;
        std::cout << "����: " << user["role"] << std::endl;
        std::cout << "-----------------------\n";
    }

    std::string editUsername;
    std::cout << "������� ����� ���������� ��� ��������������: ";
    std::cin.ignore();
    std::getline(std::cin, editUsername);


    bool found = false;
    for (auto& user : jsonData["users"]) {
        if (user["username"] == editUsername) {
            found = true;
            std::string firstName, lastName, patronymic, newUsername, newPassword, newRole;

            std::cout << "�������������� ������ ����������:\n";


            std::cout << "����� ���: ";
            std::getline(std::cin, firstName);
            user["firstName"] = firstName;

            std::cout << "����� �������: ";
            std::getline(std::cin, lastName);
            user["lastName"] = lastName;

            std::cout << "����� ��������: ";
            std::getline(std::cin, patronymic);
            user["patronymic"] = patronymic;

   
            std::cout << "����� �����: ";
            std::getline(std::cin, newUsername);
            user["username"] = newUsername;

            std::cout << "����� ������: ";
            std::getline(std::cin, newPassword);

            string newHashedPass = hashPassword(newPassword);

            user["password"] = newHashedPass;

            std::cout << "����� ����: ";
            std::getline(std::cin, newRole);
            user["role"] = newRole;

            std::cout << "������ ���������� ������� ���������.\n";
            break;
        }
    }

    if (found) {
        jsonHelper.writeJsonData("users.json", jsonData);
        Sleep(1500);
        system("cls");
    }
    else {
        std::cout << "��������� � ��������� ������� �� ������.\n";
        Sleep(1500);
        system("cls");
    }
}
void User::removeEmployees()
{
    JsonHelper jsonHelper;
    system("cls");

    json jsonData = jsonHelper.readJsonData("users.json");


    std::cout << "������ �����������:\n";
    for (const auto& user : jsonData["users"]) {
        std::cout << "���: " << user["firstName"] << std::endl;
        std::cout << "�������: " << user["lastName"] << std::endl;
        std::cout << "��������: " << user["patronymic"] << std::endl;
        std::cout << "�����: " << user["username"] << std::endl;
        std::cout << "������: " << user["password"] << std::endl;
        std::cout << "����: " << user["role"] << std::endl;
        std::cout << "-----------------------\n";
    }


    std::string deleteUsername;
    std::cout << "������� ����� ���������� ��� ��������: ";
    std::cin.ignore();
    std::getline(std::cin, deleteUsername);


    bool found = false;
    size_t index = 0;
    while (index < jsonData["users"].size()) {
        if (jsonData["users"][index]["username"] == deleteUsername) {
            found = true;
            jsonData["users"].erase(jsonData["users"].begin() + index);
            std::cout << "��������� ������� ������.\n";
            break;
        }
        else {
            index++;
        }
    }

    if (found) {
        jsonHelper.writeJsonData("users.json", jsonData);
        Sleep(1500);
        system("cls");
    }
    else {
        std::cout << "��������� � ��������� ������� �� ������.\n";
        Sleep(1500);
        system("cls");
    }
}
void User::listEmployees() {
    JsonHelper jsonHelper;
    system("cls");


    json jsonData = jsonHelper.readJsonData("users.json");

    std::vector<User> users;

    for (const auto& user : jsonData["users"]) {
        User currentUser;
        currentUser.firstName = user["firstName"];
        currentUser.lastName = user["lastName"];
        currentUser.password = user["password"];
        currentUser.patronymic = user["patronymic"];
        currentUser.role = user["role"];
        currentUser.username = user["username"];
        users.push_back(currentUser);
    }


    const int itemsPerPage = 5;
    int totalPages = (users.size() + itemsPerPage - 1) / itemsPerPage; 

    std::cout << "������ �������������:\n";
    int startIndex = 0;
    int currentPage = 1;

    while (true) {
        int endIndex = std::min(startIndex + itemsPerPage, static_cast<int>(users.size()));

        for (int i = startIndex; i < endIndex; ++i) {
            std::cout << "���: " << users[i].firstName << std::endl;
            std::cout << "�������: " << users[i].lastName << std::endl;
            std::cout << "��������: " << users[i].patronymic << std::endl;
            std::cout << "����: " << users[i].role << std::endl;
            std::cout << "��� ������������: " << users[i].username << std::endl;
            std::cout << "-----------------------\n";
        }


        bool isLastPage = currentPage == totalPages;


        if (isLastPage) {
            std::cout << "��� ��������� ��������. ��� ��������� ������� ��� ��������.\n";
            break;
        }


        std::cout << "������� 'y' ��� �������� ��������� ��������, ��� ����� ������ ������ ��� ������: ";
        std::string input;
        std::cin >> input;


        if (input != "y") {
            system("cls");
            return;
        }

        startIndex += itemsPerPage;
        currentPage++;
    }

    int choice;
    std::cout << "������� ����� ������ ��� ������: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
        break;
    }

    Sleep(1500);
    system("cls");
}

void User::displayPaidOrdersForCook() {
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");

    if (cartData["cart"].empty()) {
        cout << "��� �������." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    cout << "������, ��������� ��� �������������:\n";
    bool foundOrders = false;
    for (size_t i = 0; i < cartData["cart"].size(); ++i) {
        if (cartData["cart"][i]["status"] == "Paid for and transferred to the kitchen") {
            cout << "����� ����� " << i + 1 << ":\n";
            cout << "��������: " << cartData["cart"][i]["name"].get<string>() << endl;
            cout << "����: " << cartData["cart"][i]["cost"].get<double>() << " ������" << endl;
            cout << "--------------------------\n";
            foundOrders = true;
        }
    }

    if (!foundOrders) {
        cout << "��� ��������� �������." << endl;
    }

    int choice;
    std::cout << "������� ����� ������ ��� ������: ";
    std::cin >> choice;
    system("cls");
}
void User::startCooking() {
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");
    json menuData = jsonHelper.readJsonData("menu.json");
    json productsData = jsonHelper.readJsonData("products.json");

    if (cartData["cart"].empty()) {
        cout << "��� �������." << endl;
        Sleep(1500);
        system("cls");
        return;
    }


    cout << "������ �� �������� '�������� � �������� �� �����':\n";
    bool foundOrders = false;
    for (size_t i = 0; i < cartData["cart"].size(); ++i) {
        if (cartData["cart"][i]["status"] == "Paid for and transferred to the kitchen") {
            cout << "����� ����� " << i + 1 << ":\n";
            cout << "��������: " << cartData["cart"][i]["name"].get<string>() << endl;
            cout << "����: " << cartData["cart"][i]["cost"].get<double>() << " ������" << endl;
            cout << "--------------------------\n";
            foundOrders = true;
        }
    }

    if (!foundOrders) {
        cout << "��� ������� �� �������� ''�������� � �������� �� �����'." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    int orderNumber;
    cout << "������� ����� ������ ��� ������ �������������: ";
    cin >> orderNumber;

    if (orderNumber <= 0 || orderNumber > cartData["cart"].size()) {
        cout << "������ ������������ ����� ������." << endl;
        system("pause");
        system("cls");
        return;
    }


    string dishName = cartData["cart"][orderNumber - 1]["name"].get<string>();

    json dishData;
    for (const auto& item : menuData["menu"]) {
        if (item["name"] == dishName) {
            dishData = item;
            break;
        }
    }

    if (dishData.empty()) {
        cout << "��� ���������� � ����� � ����." << endl;
        system("pause");
        system("cls");
        return;
    }

    vector<string> ingredientNames;
    for (const auto& ingredient : dishData["ingredients"]) {
        ingredientNames.push_back(ingredient["name"].get<string>());
    }


    cout << "����������� ��� ������������� ����� '" << dishName << "':\n";
    for (const auto& ingredientName : ingredientNames) {
        cout << "- " << ingredientName << endl;
    }
    cout << "--------------------------\n";


    for (const auto& ingredientName : ingredientNames) {
        string productName;
        cout << "������� ������� ��� �������������: ";
        cin >> productName;

        bool foundProduct = false;
        for (size_t i = 0; i < productsData["products"].size(); ++i) {
            if (productsData["products"][i]["name"] == productName) {
                productsData["products"].erase(productsData["products"].begin() + i);
                foundProduct = true;
                break;
            }
        }
        if (!foundProduct) {
            cout << "������� '" << productName << "' ����������� � ������ ���������." << endl;
            system("pause");
            system("cls");
            return;
        }
    }


    cartData["cart"][orderNumber - 1]["status"] = "������� ��������� ��� ��������.";


    jsonHelper.writeJsonData("cart.json", cartData);
    jsonHelper.writeJsonData("products.json", productsData);

    cout << "������������� ����� '" << dishName << "' ���������." << endl;


    system("pause");
    system("cls");
}

void User::delivering() {
    JsonHelper jsonHelper;
    system("cls");

    json cartData = jsonHelper.readJsonData("cart.json");

    if (cartData["cart"].empty()) {
        cout << "��� �������." << endl;
        Sleep(1500);
        system("cls");
        return;
    }


    cout << "������ �� �������� '������� ��������� ��� ��������':\n";
    bool foundOrders = false;
    for (size_t i = 0; i < cartData["cart"].size(); ++i) {
        if (cartData["cart"][i]["status"] == "������� ��������� ��� ��������") {
            cout << "����� ����� " << i + 1 << ":\n";
            cout << "��������: " << cartData["cart"][i]["name"].get<string>() << endl;
            cout << "����: " << cartData["cart"][i]["cost"].get<double>() << " ������" << endl;
            cout << "--------------------------\n";
            foundOrders = true;
        }
    }

    if (!foundOrders) {
        cout << "��� ������� �� �������� '������� ��������� ��� ��������'." << endl;
        Sleep(1500);
        system("cls");
        return;
    }

    int orderNumber;
    cout << "������� ����� ������ ��� ��������� �������: ";
    cin >> orderNumber;

    if (orderNumber <= 0 || orderNumber > cartData["cart"].size()) {
        cout << "������ ������������ ����� ������." << endl;
        system("pause");
        system("cls");
        return;
    }


    cartData["cart"][orderNumber - 1]["status"] = "Done";


    jsonHelper.writeJsonData("cart.json", cartData);

    cout << "������ ������ ������� ������� �� '��������'." << endl;
    Sleep(1500);
    system("cls");
}


void User::printBalance(const std::string& filename) {
    system("cls");

    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string balanceStr;
        std::getline(inputFile, balanceStr); 

        try {
            double balance = std::stod(balanceStr); 
            std::cout << "������: " << balance << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "������ ��� ������ ������� �� �����: " << e.what() << std::endl;
        }

        inputFile.close();
    }
    else {
        std::cout << "�� ������� ������� ����: " << filename << std::endl;
    }

    int choice;
    std::cout << "������� ����� ������ ��� ������: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
    }
}
void User::printMenuCost() {
    system("cls");
    JsonHelper jsonHelper;

    json selledMenuData = jsonHelper.readJsonData("selledMenu.json");

    if (selledMenuData["selledMenu"].empty()) {
        cout << "��� ��������� ����." << endl;
        return;
    }

    double totalCost = 0.0;
    int totalCount = 0;


    cout << "��������� �����:\n";
    for (const auto& dish : selledMenuData["selledMenu"]) {
        string dishName = dish["name"].get<string>();
        double dishCost = dish["cost"].get<double>();

        cout << "�����: " << dishName << endl;
        cout << "����: " << dishCost << " ������" << endl;
        cout << "--------------------------\n";

        totalCost += dishCost;
        totalCount++;
    }

    cout << "����� ��������� ��������� ����: " << totalCost << " ������" << endl;
    cout << "���������� ��������� ����: " << totalCount << endl;

    int choice;
    std::cout << "������� ����� ������ ��� ������: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
    }
}
void User::printProductsCost() {
    system("cls");
    JsonHelper jsonHelper;

    json purchasedData = jsonHelper.readJsonData("purchasedProducts.json");

    if (purchasedData["purchasedProducts"].empty()) {
        cout << "������ ��������� ����." << endl;
        return;
    }

  
    unordered_map<string, double> productsCost;


    for (const auto& product : purchasedData["purchasedProducts"]) {
        string name = product["name"].get<string>();
        double amount = product["amount"].get<double>();


        if (productsCost.count(name) > 0) {
            productsCost[name] += amount;
        }
        else {
           
            productsCost[name] = amount;
        }
    }


    for (const auto& entry : productsCost) {
        cout << "�������: " << entry.first << endl;
        cout << "����� ���������: " << entry.second << " ������" << endl;
        cout << "-----------------------" << endl;
    }

    int choice;
    std::cout << "������� ����� ������ ��� ������: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
    }
}


void User::guestMenu() {

    system("cls");
    int choice;
    Menu menu;
    Product product;
    Guest guest;

    while (true) {
        cout << "1. ������� �����\n";
        cout << "2. �������\n";
        cout << "3. �������� �����\n";
        cout << "4. ������ ������ �������\n";
        cout << "5. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            guest.createOrder();
            break;
        case 2:
            guest.displayCart();
            break;
        case 3:
            guest.confirmOrder();
            break;
        case 4:
            guest.displayStatus();
            break;
        case 5:
            system("cls");
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            break;
        }
    }
}

void User::adminMenu() {
    int choice;
    Menu menu;
    Product product;

    while (true) {
        cout << "�������������:\n";
        cout << "1. ����\n";
        cout << "2. ������ ���������\n";
        cout << "3. ������� ������\n";
        cout << "4. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            menu.editMenu();
            break;
        case 2:
            product.editingProducts();
            break;
        case 3:
            editingEmployees();
            break;
        case 4:
            return;  
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            break;
        }
    }
}

void User::skladMenu() {

    system("cls");
    int choice;

    while (true) {
        cout << "1. ���������� �����\n";
        cout << "2. ������� ������\n";
        cout << "3. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        Menu menu;
        Product product;
        Order order;

        switch (choice) {
        case 1:
            product.productList();
            break;
        case 2:
            order.createOrder();
            break;
        case 3:
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            system("cls");
            break;
        }
    }
}

void User::supplierMenu() {
    Order order;
    system("cls");
    int choice;

    while (true) {
        cout << "1. ���������� ������\n";
        cout << "2. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            order.ordersList();
            break;
        case 2:
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            system("cls");
            break;
        }
    }
}

void User::accountantMenu() {
    Order order;
    User user;
    system("cls");
    int choice;

    while (true) {
        cout << "1. ���������� ������������ ������\n";
        cout << "2. ���������� �������� ��������\n";
        cout << "3. ���������� ������ ���������\n";
        cout << "4. ���������� �� ��������� ������\n";
        cout << "5. ���������� � �������� ���������\n";
        cout << "6. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            order.printSendOrders();
            break;
        case 2:
            order.printAcceptOrders();
            break;
        case 3:
            printBalance("RestBalance.txt");
            break;
        case 4:
            printMenuCost();
            break;
        case 5:
            printProductsCost();
            break;
        case 6:
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            system("cls");
            break;
        }
    }
}

void User::povarMenu() {
    
    system("cls");
    int choice;

    while (true) {
        cout << "1. ������ �������\n";
        cout << "2. ������ ��������\n";
        cout << "3. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            displayPaidOrdersForCook();
            break;
        case 2:
            startCooking();
            break;
        case 3:
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            system("cls");
            break;
        }
    }
}

void User::oficMenu() {
    system("cls");
    int choice;

    while (true) {
        cout << "1. ������ ������������\n";
        cout << "2. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "������������ �����. ���������� ��� ���." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            delivering();
            break;
        case 2:
            return;
        default:
            cout << "������������ �����. ���������� ��� ���." << endl;
            system("cls");
            break;
        }
    }
}








