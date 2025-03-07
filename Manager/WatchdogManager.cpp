#include "WatchdogManager.h"
#include <chrono>
#include <cstdlib>

WatchdogManager::WatchdogManager(const std::string& ip, std::atomic<bool>& reachableFlag)
    : ipAddress(ip), isReachable(reachableFlag) {}

void WatchdogManager::start() {
    // Launch a dedicated thread that pings every 2 seconds
    watchdogThread = std::thread([this]() {
        while (running) {
            bool reachable = checkReachability(ipAddress);
            isReachable.store(reachable);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });
}

void WatchdogManager::stop() {
    running = false;
    if (watchdogThread.joinable()) {
        watchdogThread.join();
    }
}

bool WatchdogManager::checkReachability(const std::string& ip) {
    std::string cmd = "ping -c 1 " + ip + " > /dev/null 2>&1";
    return (system(cmd.c_str()) == 0);
}
