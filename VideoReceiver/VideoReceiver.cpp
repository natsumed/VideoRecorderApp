#include "VideoReceiver.h"
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QDebug>

VideoReceiver::VideoReceiver(QObject *parent) : QObject(parent) {
    gst_init(nullptr, nullptr);

    pipeline = gst_pipeline_new("video-receiver");
    GstElement *udpsrc = gst_element_factory_make("udpsrc", "udpsrc");
    GstElement *rtph264depay = gst_element_factory_make("rtph264depay", "rtph264depay");
    GstElement *avdec_h264 = gst_element_factory_make("avdec_h264", "avdec_h264");
    GstElement *videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    videosink = gst_element_factory_make("xvimagesink", "videosink");  // Use xvimagesink or glimagesink

    if (!pipeline || !udpsrc || !rtph264depay || !avdec_h264 || !videoconvert || !videosink) {
        qDebug() << "Not all elements could be created.";
        return;
    }

    g_object_set(udpsrc, "port", 5600, nullptr);  // Remove the latency property
    GstCaps *caps = gst_caps_new_simple("application/x-rtp",
                                        "media", G_TYPE_STRING, "video",
                                        "encoding-name", G_TYPE_STRING, "H264",
                                        "payload", G_TYPE_INT, 96,
                                        nullptr);
    g_object_set(udpsrc, "caps", caps, nullptr);
    gst_caps_unref(caps);

    gst_bin_add_many(GST_BIN(pipeline), udpsrc, rtph264depay, avdec_h264, videoconvert, videosink, nullptr);
    if (!gst_element_link_many(udpsrc, rtph264depay, avdec_h264, videoconvert, videosink, nullptr)) {
        qDebug() << "Elements could not be linked.";
        gst_object_unref(pipeline);
        return;
    }

    g_object_set(videosink, "sync", FALSE, nullptr);  // Disable sync to reduce latency

    GstBus *bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, (GstBusFunc)bus_call, this);
    gst_object_unref(bus);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

VideoReceiver::~VideoReceiver() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

void VideoReceiver::setWindowId(WId id) {
    if (videosink != nullptr) {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), id);
    }
}

gboolean VideoReceiver::bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            qDebug() << "End of stream";
            break;

        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);
            g_free(debug);

            qDebug() << "Error:" << error->message;
            g_error_free(error);
            break;
        }
        default:
            break;
    }

    return TRUE;
}
