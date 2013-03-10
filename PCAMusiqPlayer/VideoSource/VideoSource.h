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

typedef struct Frame_8UC3
{
    unsigned char* data;
    int width;
    int height;
} Frame_8UC3;

class VideoSourceFrameReceiver : public QObject
{
public:
    //virtual void newFrame(cv::Mat frame/*, ColorFormat colorFormat*/) = 0;
    virtual void newFrame(Frame_8UC3 frame) = 0;
};

#endif // VIDEOSOURCE_H
