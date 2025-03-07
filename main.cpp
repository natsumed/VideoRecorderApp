// main.cpp
#include <QApplication>
#include "VideoWindow.h"
#include "ConfigManager.h"
#include "VideoSourceConfig.h"
#include "WatchdogManager.h"
#include <iostream>
#include <atomic>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        // Use an absolute path to the config.json file
        VideoSourceConfig config = ConfigManager::loadConfig("../config.json");

        std::atomic<bool> isReachable{true}; // Placeholder for actual reachability check
        WatchdogManager watchdog(config.ip, isReachable);

        VideoWindow window;
        window.show();

        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
