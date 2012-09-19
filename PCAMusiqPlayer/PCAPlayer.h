#ifndef PCAPLAYER_H
#define PCAPLAYER_H

#include <QObject>
#include <QThread>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "TimeSeries.h"

class PCAPlayer : public QThread
{
    Q_OBJECT

public:

signals:
    void newFrame(QImage frame);
    void newReconstructedFrame(QImage reconstructedFrame);
    void newCoefficients(TimeSeriesSamples coefficients);

public:
    PCAPlayer();
    void run();

private:
    QImage IplImage2QImage(IplImage *iplImage);
    QImage cvMat2QImage(cv::Mat m, int width, int height);

    IplImage* reconstructImage(TimeSeriesSamples coefficients);
};

#endif // PCAPLAYER_H
