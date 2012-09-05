#include "PCAPlayer.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QImage>

PCAPlayer::PCAPlayer()
{

}

void PCAPlayer::run()
{
    qDebug() << "hello from PCAPlayer thread";

    QString videofilename = QApplication::instance()->arguments()[1];
    qDebug() << "Opening " << videofilename;

    CvCapture* capture = cvCaptureFromFile(videofilename.toStdString().c_str());
    cvQueryFrame(capture);
    int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    int fps       = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int numFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);

    qDebug() << frameW << " x " << frameH;

    while(1)
    {
        IplImage* img = 0;
        if(!cvGrabFrame(capture)){              // capture a frame
          printf("Could not grab a frame\n\7");
          //exit(0);
        }
        img = cvRetrieveFrame(capture);           // retrieve the captured frame

        //uchar* data;
        QImage newImage = this->IplImage2QImage(img);
        newFrame(newImage);
        qDebug() << "emitted a newImage signal";
    }
}

QImage PCAPlayer::IplImage2QImage(IplImage *iplImage) {



    QImage image = QImage(iplImage->width, iplImage->height, QImage::Format_RGB888);
    QImage* qtImage = &image;

        // Assume IplImage having 3 channels (RGB)

        //int channels = iplImage->nChannels;
        char *data = iplImage->imageData;
        char r, g, b, a = 0;

        for (int y=0; y < qtImage->height(); y++, data += iplImage->widthStep)

                for (int x=0; x < qtImage->width(); x++) {

                        b = data[x * 3 + 0];
                        g = data[x * 3 + 1];
                        r = data[x * 3 + 2];

                        qtImage->setPixel(x, y, qRgb(r,g,b));

                }
        return image;
}
