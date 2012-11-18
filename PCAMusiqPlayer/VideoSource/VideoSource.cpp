#include "VideoSource.h"

VideoSource::VideoSource(VideoSourceFrameReceiver *receiver)
{
    this->receiver = receiver;
}
