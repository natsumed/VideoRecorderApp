// ConfigManager.h
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include "VideoSourceConfig.h"

class ConfigManager {
public:
    static VideoSourceConfig loadConfig(const std::string& filePath);
};

#endif // CONFIGMANAGER_H
