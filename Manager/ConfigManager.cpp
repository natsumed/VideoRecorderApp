// ConfigManager.cpp
#include "ConfigManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

VideoSourceConfig ConfigManager::loadConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        throw std::runtime_error("Could not open config file");
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Error: File " << filePath << " is empty" << std::endl;
        throw std::runtime_error("Config file is empty");
    }

    try {
        nlohmann::json configJson;
        file >> configJson;

        VideoSourceConfig config;
        config.ip = configJson.at("ip").get<std::string>();
        config.port = configJson.at("port").get<int>();
        config.resolution = configJson.at("resolution").get<std::string>();
        config.frameRate = configJson.at("frameRate").get<int>();

        return config;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        throw;
    } catch (const nlohmann::json::out_of_range& e) {
        std::cerr << "Config error: " << e.what() << std::endl;
        throw;
    }
}
