// VideoSourceConfig.h
#ifndef VIDEOSOURCECONFIG_H
#define VIDEOSOURCECONFIG_H

#include <string>

struct VideoSourceConfig {
    std::string ip;
    int port;
    std::string resolution;
    int frameRate;
};

#endif // VIDEOSOURCECONFIG_H
