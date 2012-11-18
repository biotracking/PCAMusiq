#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <QObject>

#include <opencv/cv.h>

enum ColorFormat
{
    RGB, BGR
};

class VideoSourceFrameReceiver;

class VideoSource
{
public:
    VideoSource(VideoSourceFrameReceiver* receiver);
    virtual void start() = 0;
protected:
    VideoSourceFrameReceiver* receiver;
};

class VideoSourceFrameReceiver : public QObject
{
public:
    virtual void newFrame(cv::Mat frame/*, ColorFormat colorFormat*/) = 0;
};

#endif // VIDEOSOURCE_H
