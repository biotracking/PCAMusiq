#include "VideoSourceFile.h"

#include <QString>

VideoSourceFile::VideoSourceFile(QString filePath, VideoSourceFrameReceiver *receiver)
    : VideoSourceOpenCV(receiver)
{
    std::cout<<"Opening video:"<<filePath.toStdString()<<std::endl;
    capture.open(filePath.toStdString().c_str());
    std::cout<<"Opened video"<<std::endl;
}
