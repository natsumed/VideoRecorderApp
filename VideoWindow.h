#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include "VideoReceiver.h"

class VideoWindow : public QWidget {
    Q_OBJECT

public:
    explicit VideoWindow(QWidget *parent = nullptr);
    ~VideoWindow();

protected:
    void showEvent(QShowEvent *event) override;

private:
    VideoReceiver *receiver;
};

#endif // VIDEOWINDOW_H
