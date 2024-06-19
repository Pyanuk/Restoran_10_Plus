#pragma once

#include <string>

class Product {
public:
    int id;
    std::string name;
    double price;

    void addProduct();
    void editProduct();
    void removeProduct();
    void editingProducts();
    void productList();
};