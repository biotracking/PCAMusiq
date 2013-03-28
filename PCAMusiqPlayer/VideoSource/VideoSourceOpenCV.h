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
/*    Q_OBJECT

public:

signals:
    //void newFrame(QImage frame); */

public:
    VideoSourceOpenCV(VideoSourceFrameReceiver* receiver);
    void start() { QThread::start(); }
    void run();

protected:
    cv::VideoCapture capture;

private:

    QImage IplImage2QImage(IplImage *iplImage);
    QImage cvMat2QImage(cv::Mat m, int width, int height);
    float maxMatf(cv::Mat m);
};

#endif // VIDEOSOURCEOPENCV_H
