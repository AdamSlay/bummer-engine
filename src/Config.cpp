#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "Config.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void loadConfig(const std::string& path) {
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;

    SCREEN_WIDTH = j["SCREEN_WIDTH"];
    SCREEN_HEIGHT = j["SCREEN_HEIGHT"];
}
