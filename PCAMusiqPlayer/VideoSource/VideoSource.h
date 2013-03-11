#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <QObject>

#include <opencv/cv.h>

#include <tr1/memory>

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

class Frame_8UC3
{
public:
    Frame_8UC3(int width, int height)
    {
        this->width = width;
        this->height = height;
        data.resize(width * height * 3);
    }

    std::vector<unsigned char> data;
    int width;
    int height;

private:
    Frame_8UC3() { }
};

class VideoSourceFrameReceiver : public QObject
{
public:
    //virtual void newFrame(cv::Mat frame/*, ColorFormat colorFormat*/) = 0;
    virtual void newFrame(Frame_8UC3 frame) = 0;
};

#endif // VIDEOSOURCE_H
