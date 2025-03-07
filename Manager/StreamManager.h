#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <atomic>
#include <thread>
#include <string>
#include <gst/gst.h>

class StreamManager {
public:
    StreamManager(std::atomic<bool>& reachableFlag,
                  const std::string& remoteUrl,
                  const std::string& fallbackPath);
    void start();
    void stop();

private:
    void playStream(const std::string& streamUrl);
    static void on_pad_added(GstElement *src, GstPad *new_pad, gpointer data);

    std::atomic<bool>& isReachable;
    std::string url;
    std::string localVideo;
    std::thread streamingThread;
    bool running = true;
    GstElement *pipeline = nullptr;
};

#endif // STREAMMANAGER_H
