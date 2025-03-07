#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <atomic>
#include <thread>
#include <string>

class WatchdogManager {
public:
    WatchdogManager(const std::string& ip, std::atomic<bool>& reachableFlag);
    void start();
    void stop();

private:
    bool checkReachability(const std::string& ip);

    std::string ipAddress;
    std::atomic<bool>& isReachable;
    std::thread watchdogThread;
    bool running = true;
};

#endif // WATCHDOGMANAGER_H
