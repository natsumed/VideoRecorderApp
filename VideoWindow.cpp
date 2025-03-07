#include "VideoWindow.h"
#include <QVBoxLayout>

VideoWindow::VideoWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    receiver = new VideoReceiver(this);
    setLayout(layout);

    // Set a default size for the window
    resize(800, 600);  // Adjust the width and height as needed
}

VideoWindow::~VideoWindow() {
    delete receiver;
}

void VideoWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    receiver->setWindowId(winId());
}
