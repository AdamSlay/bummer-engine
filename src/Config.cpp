#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "Config.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file){
        std::cerr << "Unable to open Config file: " << path << std::endl;

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            std::cerr << "Current working dir: " << cwd << std::endl;
        }
        return;
    }
    nlohmann::json j;
    file >> j;

    SCREEN_WIDTH = j["SCREEN_WIDTH"];
    SCREEN_HEIGHT = j["SCREEN_HEIGHT"];
}
