#ifndef PROSILICACAMERA_H
#define PROSILICACAMERA_H

#include "VideoSource.h"

class VideoSourceProsilicaCamera : public VideoSource
{
public:
    VideoSourceProsilicaCamera(VideoSourceFrameReceiver* receiver);

    void start();
    void stop();
};

#endif // PROSILICACAMERA_H
