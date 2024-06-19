#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include "single_include/nlohmann/json.hpp"

using json = nlohmann::json;

class JsonHelper {
public:
    static json readJsonData(const std::string& filename);
    static void writeJsonData(const std::string& filename, const json& jsonData);
    static json getProductData(int productId);
};

#endif  
