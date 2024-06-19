#pragma once

#include <string>

class User {
public:
    std::string role;
    std::string lastName;
    std::string firstName;
    std::string patronymic;
    std::string username;
    std::string password;

    void guestMenu();
    void skladMenu();
    void adminMenu();
    void povarMenu();
    void supplierMenu();
    void accountantMenu();
    void oficMenu();

    void editEmployees();
    void addEmployees();
    void removeEmployees();
    void editingEmployees();
    void listEmployees();
    
    void printBalance(const std::string& filename);
    void printMenuCost();
    void printProductsCost();

    void displayPaidOrdersForCook();
    void startCooking();
    void delivering();
};
