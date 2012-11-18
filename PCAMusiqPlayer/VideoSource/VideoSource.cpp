#include "VideoSource.h"

VideoSource::VideoSource(VideoSourceFrameReceiver *sink)
{
    this->sink = sink;
}
