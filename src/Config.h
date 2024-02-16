#pragma once

#ifndef BUMMERENGINE_CONFIG_H
#define BUMMERENGINE_CONFIG_H

#include <string>
#include <nlohmann/json.hpp>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

void loadConfig(const std::string& path);

#endif //BUMMERENGINE_CONFIG_H
