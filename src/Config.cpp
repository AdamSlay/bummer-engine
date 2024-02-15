#include "Config.h"
#include <string>
#include <fstream>

Config* Config::instance = 0;

Config::Config() {
    std::ifstream configFile("../etc/run_config.json");
    configFile >> configData;
}

Config* Config::getInstance() {
    if (!instance)
        instance = new Config;
    return instance;
}

int Config::getScreenWidth() {
    return configData["SCREEN_WIDTH"];
}

int Config::getScreenHeight() {
    return configData["SCREEN_HEIGHT"];
}