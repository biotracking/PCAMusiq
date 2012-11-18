#ifndef PROSILICACAMERA_H
#define PROSILICACAMERA_H

#include <PvApi.h>
#include "VideoSource.h"

class VideoSourceProsilicaCamera : public VideoSource
{
public:
    VideoSourceProsilicaCamera(VideoSourceFrameReceiver* receiver);

    void start();
    void stop();

    void frameCallback(tPvFrame* pFrame);
};

#endif // PROSILICACAMERA_H
