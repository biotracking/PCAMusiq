#pragma once

#include <QObject>
#include <QImage>
#include <QThread>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoSourceOpenCV.h"

class VideoSourceFile : public VideoSourceOpenCV
{

public:
    VideoSourceFile(QString filePath, VideoSourceFrameReceiver* receiver);

};

