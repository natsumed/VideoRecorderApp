#include "StreamManager.h"
#include <QDebug>

StreamManager::StreamManager(std::atomic<bool>& reachableFlag,
                             const std::string& remoteUrl,
                             const std::string& fallbackPath)
    : isReachable(reachableFlag), url(remoteUrl), localVideo(fallbackPath) {}

void StreamManager::start() {
    // Run in a separate thread or in the main loop
    streamingThread = std::thread([this]() {
        bool lastState = false;
        while (true) {
            bool currentState = isReachable.load();
            if (currentState != lastState) {
                if (currentState) {
                    playStream(url);
                } else {
                    playStream(localVideo);
                }
                lastState = currentState;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void StreamManager::stop() {
    running = false;
    if (streamingThread.joinable()) {
        streamingThread.join();
    }
}

void StreamManager::playStream(const std::string& streamUrl) {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }

    pipeline = gst_pipeline_new("video-receiver");
    GstElement *source = gst_element_factory_make("uridecodebin", "source");
    GstElement *videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    GstElement *videosink = gst_element_factory_make("xvimagesink", "videosink");

    if (!pipeline || !source || !videoconvert || !videosink) {
        qDebug() << "Not all elements could be created.";
        return;
    }

    g_object_set(source, "uri", streamUrl.c_str(), nullptr);

    gst_bin_add_many(GST_BIN(pipeline), source, videoconvert, videosink, nullptr);
    gst_element_link_many(videoconvert, videosink, nullptr);

    g_signal_connect(source, "pad-added", G_CALLBACK(on_pad_added), videoconvert);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void StreamManager::on_pad_added(GstElement *src, GstPad *new_pad, gpointer data) {
    GstElement *sink = GST_ELEMENT(data);
    GstPad *sink_pad = gst_element_get_static_pad(sink, "sink");
    if (gst_pad_is_linked(sink_pad)) {
        g_object_unref(sink_pad);
        return;
    }

    GstCaps *new_pad_caps = gst_pad_get_current_caps(new_pad);
    GstStructure *new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    const gchar *new_pad_type = gst_structure_get_name(new_pad_struct);

    if (g_str_has_prefix(new_pad_type, "video/x-raw")) {
        if (gst_pad_link(new_pad, sink_pad) != GST_PAD_LINK_OK) {
            qDebug() << "Type is not compatible.";
        }
    }

    if (new_pad_caps != nullptr) {
        gst_caps_unref(new_pad_caps);
    }

    g_object_unref(sink_pad);
}
