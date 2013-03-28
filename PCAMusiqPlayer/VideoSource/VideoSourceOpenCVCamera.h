#include "VideoSourceOpenCV.h"

#include <QThread>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef VIDEOSOURCEOPENCVCAMERA_H
#define VIDEOSOURCEOPENCVCAMERA_H

class VideoSourceOpenCVCamera : public VideoSourceOpenCV
{

    public:
        VideoSourceOpenCVCamera(VideoSourceFrameReceiver* receiver);

};

#endif // VIDEOSOURCEOPENCVCAMERA_H
