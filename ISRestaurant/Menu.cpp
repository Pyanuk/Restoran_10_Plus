#include "Menu.h"
#include "Product.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <cstdlib>
#include "JsonHelper.h"
#define NOMINMAX
#include <windows.h>

using namespace std;

void Menu::editMenu() {
    system("cls");
    int choice;

    while (true) {
        cout << "Редактирование меню:\n";
        cout << "1. Список блюд\n";
        cout << "2. Добавить блюдо\n";
        cout << "3. Удалить блюдо\n";
        cout << "4. Редактировать блюдо\n";
        cout << "5. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Неправильный выбор. Попробуйте еще раз." << endl;
            cin.clear();
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            listDishes();
            break;
        case 2:
            addDish();
            editMenu();
            break;
        case 3:
            removeDish();
            break;
        case 4:
            editDish();
            break;
        case 5:
            system("cls");
            return;  
            break; 
        default:
            cout << "Неправильный выбор. Попробуйте еще раз." << endl;
            system("cls");
            break;
        }
    }
}

void Menu::addDish() {
    JsonHelper jsonHelper;

    system("cls");

    Menu newDish;

    json productsData = jsonHelper.readJsonData("products.json");

    cout << "Введите наименование блюда: ";
    cin.ignore();
    getline(cin, newDish.name);

    cout << "Введите граммовку блюда: ";
    getline(cin, newDish.weight);

    char choice;
    do {

        Product product;
        cout << "Введите ID продукта: ";
        cin >> product.id;

        bool found = false;

        for (const auto& selectedProduct : productsData["products"]) {
            if (selectedProduct["id"] == product.id) {
                found = true;

                Product newIngredient;
                newIngredient.id = selectedProduct["id"];
                newIngredient.name = selectedProduct["name"];
                newIngredient.price = selectedProduct["price"];
                newDish.ingredients.push_back(newIngredient);
                break;
            }
        }

        if (!found) {
            cout << "Продукт с указанным ID не найден." << endl;

        }

        cout << "Добавить еще ингредиенты? (y/n): ";
        cin >> choice;
    } while (choice == 'y');

    cout << "Введите описание блюда: ";
    cin.ignore();
    getline(cin, newDish.description);

    cout << "Введите время приготовления блюда: ";
    getline(cin, newDish.cookingTime);

    cout << "Введите стоимость блюда: ";
    cin >> newDish.cost;

    json menuData = jsonHelper.readJsonData("menu.json");


    json dishJson;

    dishJson["name"] = newDish.name;
    dishJson["weight"] = newDish.weight;

    json ingredientsJson = json::array();
    for (const auto& ingredient : newDish.ingredients) {
        json ingredientJson = {
            {"id", ingredient.id},
            {"name", ingredient.name},
            {"price", ingredient.price}
        };
        ingredientsJson.push_back(ingredientJson);
    }

    dishJson["ingredients"] = ingredientsJson;

    dishJson["description"] = newDish.description;
    dishJson["cookingTime"] = newDish.cookingTime;
    dishJson["cost"] = newDish.cost;

    menuData["menu"].push_back(dishJson);

    jsonHelper.writeJsonData("menu.json", menuData);

    cout << "Блюдо успешно добавлено!" << endl;


    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    system("pause");
    system("cls");
}

void Menu::removeDish()
{
    JsonHelper jsonHelper;

    system("cls");


    json jsonData = jsonHelper.readJsonData("menu.json");

    if (jsonData["menu"].empty()) {
        cout << "Меню пусто. Нет записей для удаления." << endl;
        system("pause");
        system("cls");
        return;
    }


    cout << "Текущее меню:\n";
    for (size_t i = 0; i < jsonData["menu"].size(); ++i) {
        cout << "Запись " << i + 1 << ":\n";
        cout << "Название: " << jsonData["menu"][i]["name"].get<string>() << endl;
        cout << "Описание: " << jsonData["menu"][i]["description"].get<string>() << endl;
        cout << "--------------------------\n";
    }


    int recordNumber;
    cout << "Введите номер записи для удаления: ";
    cin >> recordNumber;


    if (recordNumber <= 0 || recordNumber > jsonData["menu"].size()) {
        cout << "Введен некорректный номер записи." << endl;
        system("pause");
        system("cls");
        return;
    }

    jsonData["menu"].erase(jsonData["menu"].begin() + recordNumber - 1);

    jsonHelper.writeJsonData("menu.json", jsonData);

    cout << "Запись успешно удалена!" << endl;

    system("pause");
    system("cls");
}

void Menu::editDish()
{
    JsonHelper jsonHelper;

    system("cls");

    json jsonData = jsonHelper.readJsonData("menu.json");
    json productsData = jsonHelper.readJsonData("products.json");


    if (jsonData["menu"].empty()) {
        cout << "Меню пусто. Нет записей для редактирования." << endl;
        system("pause");
        system("cls");
        return;
    }

    cout << "Текущее меню:\n";
    for (size_t i = 0; i < jsonData["menu"].size(); ++i) {
        cout << "Запись " << i + 1 << ":\n";
        cout << "Название: " << jsonData["menu"][i]["name"].get<string>() << endl;
        cout << "Описание: " << jsonData["menu"][i]["description"].get<string>() << endl;
        cout << "--------------------------\n";
    }

    int recordNumber;
    cout << "Введите номер записи для редактирования: ";
    cin >> recordNumber;


    if (recordNumber <= 0 || recordNumber > jsonData["menu"].size()) {
        cout << "Введен некорректный номер записи." << endl;
        system("pause");
        system("cls");
        return;
    }

    json& selectedDish = jsonData["menu"][recordNumber - 1];

    cout << "Редактирование блюда:\n";
    cout << "Название: " << selectedDish["name"].get<string>() << endl;
    cout << "Описание: " << selectedDish["description"].get<string>() << endl;

    cin.ignore();
    cout << "Введите новое название блюда: ";
    string newName;
    std::getline(cin, newName);
    selectedDish["name"] = newName;

    cout << "Введите новое описание блюда: ";
    string newDescription;
    std::getline(cin, newDescription);
    selectedDish["description"] = newDescription;

    cout << "Введите новую стоимость блюда: ";
    double newCost;
    cin >> newCost;
    selectedDish["cost"] = newCost;

    cout << "Введите новое время приготовления блюда: ";
    string newCookingTime;
    cin.ignore();
    std::getline(cin, newCookingTime);
    selectedDish["cookingTime"] = newCookingTime;

    cout << "Введите новую граммовку блюда: ";
    string newWeight;
    std::getline(cin, newWeight);
    selectedDish["weight"] = newWeight;


    selectedDish["ingredients"].clear();

    char choice;
    do {

        Product product;
        cout << "Введите ID продукта: ";
        cin >> product.id;

        bool found = false;

        for (const auto& availableProduct : productsData["products"]) {
            if (availableProduct["id"] == product.id) {
                found = true;
        
                Product newIngredient;
                newIngredient.id = availableProduct["id"];
                newIngredient.name = availableProduct["name"];
                newIngredient.price = availableProduct["price"];
                selectedDish["ingredients"].push_back(availableProduct);
                break;
            }
        }

        if (!found) {
            cout << "Продукт с указанным ID не найден." << endl;
            
        }

        cout << "Добавить еще ингредиенты? (y/n): ";
        cin >> choice;
    } while (choice == 'y');

    jsonHelper.writeJsonData("menu.json", jsonData);

    cout << "Запись успешно отредактирована!" << endl;

    system("pause");
    system("cls");
}

void Menu::listDishes() {
    JsonHelper jsonHelper;
    system("cls");


    json jsonData = jsonHelper.readJsonData("menu.json");


    std::vector<Menu> dishes;


    for (const auto& dish : jsonData["menu"]) {
        Menu currentDish;
        currentDish.name = dish["name"];
        currentDish.description = dish["description"];
        currentDish.cookingTime = dish["cookingTime"];
        currentDish.weight = dish["weight"];
        currentDish.cost = dish["cost"];

        for (const auto& ingredient : dish["ingredients"]) {
            Product currentIngredient;
            currentIngredient.id = ingredient["id"];
            currentIngredient.name = ingredient["name"];
            currentIngredient.price = ingredient["price"];
            currentDish.ingredients.push_back(currentIngredient);
        }

        dishes.push_back(currentDish);
    }

    const int itemsPerPage = 5;
    int totalPages = (dishes.size() + itemsPerPage - 1) / itemsPerPage; 

    std::cout << "Список блюд:\n";
    int startIndex = 0;
    int currentPage = 1;

    while (true) {
        int endIndex = std::min(startIndex + itemsPerPage, static_cast<int>(dishes.size()));

        for (int i = startIndex; i < endIndex; ++i) {
            std::cout << "Название: " << dishes[i].name << std::endl;
            std::cout << "Описание: " << dishes[i].description << std::endl;
            std::cout << "Время приготовления: " << dishes[i].cookingTime << std::endl;
            std::cout << "Вес: " << dishes[i].weight << std::endl;
            std::cout << "Стоимость: " << dishes[i].cost << std::endl;

            std::cout << "Ингредиенты:\n";
            for (const auto& ingredient : dishes[i].ingredients) {
                std::cout << "  - Название: " << ingredient.name << std::endl;
                std::cout << "    Цена: " << ingredient.price << std::endl;
            }
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
            system("cls");
            return;
        }


        startIndex += itemsPerPage;
        currentPage++;
    }

    int choice;
    std::cout << "Введите любой символ для выхода: ";
    std::cin >> choice;
    switch (choice) {
    default:
        return;
        break;
    }

    Sleep(1500);
    system("cls");
}

