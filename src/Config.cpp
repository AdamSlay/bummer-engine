#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "Config.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int VIRTUAL_WIDTH;
int VIRTUAL_HEIGHT;
std::string FONT_PATH;
std::string SCANCODE_MAP_PATH;
std::string CONTROLLER_MAP_PATH;
int FONT_SIZE;
int CAMERA_X;
int CAMERA_Y;

void loadConfig(const std::string& path) {
    /**
     * Load the configuration from a file
     *
     * @param path: The path to the configuration file
     */
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
    VIRTUAL_WIDTH = j["VIRTUAL_WIDTH"];
    VIRTUAL_HEIGHT = j["VIRTUAL_HEIGHT"];
    FONT_PATH = j["FONT_PATH"].get<std::string>();
    FONT_SIZE = j["FONT_SIZE"];
    SCANCODE_MAP_PATH = j["SCANCODE_MAP_PATH"];
    CONTROLLER_MAP_PATH = j["CONTROLLER_MAP_PATH"];
    CAMERA_X = j["CAMERA_X"];
    CAMERA_Y = j["CAMERA_Y"];
}
