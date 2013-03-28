#include "VideoSourceOpenCV.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QDateTime>

#include "Config.h"
#include "MathUtil.h"
#include "CVUtil.h"

VideoSourceOpenCV::VideoSourceOpenCV(VideoSourceFrameReceiver *receiver)
    : VideoSource(receiver)
    , QThread()
{
    std::cout<<"Construct VideoSourceOpenCV"<<std::endl;
}

void VideoSourceOpenCV::run()
{
    cv::Mat img;
    qint64 frameDuration = (qint64) (1000.0 / capture.get(CV_CAP_PROP_FPS));

    qint64 lastTime = QDateTime::currentMSecsSinceEpoch() - frameDuration,
           currentTime;
    while(1)
    {
        currentTime = QDateTime::currentMSecsSinceEpoch();
        if(currentTime - lastTime < frameDuration)
        {
            unsigned long sleepTime = 1000 * (frameDuration - (currentTime - lastTime));
            usleep(sleepTime);
        }

        capture.read(img);

        Frame_8UC3 frame(img.cols, img.rows);

        unsigned char* data = (unsigned char*) img.data;
        for(int x = 0; x < frame.width; x++)
        {
            for(int y = 0; y < frame.height; y++)
            {
                int componentIndex = y * img.step[0] + x * img.step[1];

                frame.data[y * frame.width * 3 + x * 3 + 0] = data[componentIndex + 0];
                frame.data[y * frame.width * 3 + x * 3 + 1] = data[componentIndex + 1];
                frame.data[y * frame.width * 3 + x * 3 + 2] = data[componentIndex + 2];
            }
        }

        this->receiver->newFrame(frame);

        lastTime = currentTime;
    }
}

size_t componentCount(cv::Mat m)
{
    size_t count = 1;
    for(int d = 0; d < m.dims; d++)
        count *= m.size[d];
    return count;
}

float VideoSourceOpenCV::maxMatf(cv::Mat m)
{
    float max = 0.0;

    for(unsigned int v = 0; v < componentCount(m); v++)
    {
        max = MAX(m.at<float>(v), max);
    }

    return max;
}

QImage VideoSourceOpenCV::cvMat2QImage(cv::Mat m, int width, int height)
{
    QImage image = QImage(width, height, QImage::Format_RGB888);


    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            int pixelIndex = y * width + x;

#ifdef BLACK_AND_WHITE
            float r = m.at<float>(pixelIndex);
            float g = r;
            float b = r;

#else
            float b = m.at<float>(pixelIndex * 3 + 0);
            float g = m.at<float>(pixelIndex * 3 + 1);
            float r = m.at<float>(pixelIndex * 3 + 2);
#endif

#ifdef CLAMP_RECONSTRUCTED_COLOR
            // getting some r, g, b's outside 0.0 to 255.0 (surely because of the information loss from PCA)
            r = CLAMP(r, 0.0, 255.0);
            g = CLAMP(g, 0.0, 255.0);
            b = CLAMP(b, 0.0, 255.0);
#endif

            image.setPixel(x, y, qRgb(r,g,b));
        }
    }


    return image;
}

QImage VideoSourceOpenCV::IplImage2QImage(IplImage *iplImage) {

    assert(iplImage->depth == IPL_DEPTH_8U);

    QImage image = QImage(iplImage->width, iplImage->height, QImage::Format_RGB888);
    QImage* qtImage = &image;

    //int channels = iplImage->nChannels;
    char *data = iplImage->imageData;
    char r, g, b;

    for (int y=0; y < qtImage->height(); y++, data += iplImage->widthStep)
    {
        for (int x=0; x < qtImage->width(); x++)
        {
            switch(iplImage->nChannels)
            {
            case 3:
                b = data[x * 3 + 0];
                g = data[x * 3 + 1];
                r = data[x * 3 + 2];
                qtImage->setPixel(x, y, qRgb(r,g,b));
                break;
            case 1:
                char v = data[x];
                qtImage->setPixel(x, y, qRgb(v, v, v));
                break;
            }
        }
    }

    return image;
}

