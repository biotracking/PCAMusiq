#include "VideoSourceOpenCVCamera.h"

VideoSourceOpenCVCamera::VideoSourceOpenCVCamera(VideoSourceFrameReceiver *receiver)
    : VideoSourceOpenCV(receiver)
{
    capture.open(0);
}
