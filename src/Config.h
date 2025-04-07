#pragma once

#ifndef BUMMERENGINE_CONFIG_H
#define BUMMERENGINE_CONFIG_H

#include <string>
#include <nlohmann/json.hpp>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int VIRTUAL_WIDTH;
extern int VIRTUAL_HEIGHT;
extern std::string FONT_PATH;
extern std::string SCANCODE_MAP_PATH;
extern std::string CONTROLLER_MAP_PATH;
extern int FONT_SIZE;
extern int CAMERA_X;
extern int CAMERA_Y;

void loadConfig(const std::string& path);

#endif //BUMMERENGINE_CONFIG_H
