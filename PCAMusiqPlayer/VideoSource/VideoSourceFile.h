#pragma once

#include <QObject>
#include <QImage>
#include <QThread>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoSource.h"

class VideoSourceFile : public VideoSource, public QThread
{
/*    Q_OBJECT

public:

signals:
    //void newFrame(QImage frame); */

public:
    VideoSourceFile(QString filePath, VideoSourceFrameReceiver* receiver);
    void start() { QThread::start(); }
    void run();

private:

    QImage IplImage2QImage(IplImage *iplImage);
    QImage cvMat2QImage(cv::Mat m, int width, int height);
    float maxMatf(cv::Mat m);

    cv::VideoCapture capture;
};

