#ifndef VIDEOSOURCEOPENCV_H
#define VIDEOSOURCEOPENCV_H

#include <QObject>
#include <QImage>
#include <QThread>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoSource.h"

class VideoSourceOpenCV : public VideoSource, public QThread
{

public:
    void start() { QThread::start(); }
    void run();

protected:
    // dont't instantiate this class. must pick a subclass, either VideoSourceOpenCVCamera or VideoSourceFile
    // at time of writing
    VideoSourceOpenCV(VideoSourceFrameReceiver* receiver);

    cv::VideoCapture capture;

private:

    QImage IplImage2QImage(IplImage *iplImage);
    QImage cvMat2QImage(cv::Mat m, int width, int height);
    float maxMatf(cv::Mat m);
};

#endif // VIDEOSOURCEOPENCV_H
