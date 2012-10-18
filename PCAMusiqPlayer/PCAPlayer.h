#ifndef PCAPLAYER_H
#define PCAPLAYER_H

#include <QObject>
#include <QThread>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "TimeSeries.h"
#include "PCA.h"

class PCAPlayer : public QThread
{
    Q_OBJECT

public:

signals:
    void newFrame(QImage frame);
    void newReconstructedFrame(QImage reconstructedFrame);
    void newCoefficients(TimeSeriesSamples coefficients);
    void newNormalizedCoefficients(TimeSeriesSamples coefficients);

public:
    PCAPlayer();
    QVector<QImage> eigenFrames();
    void run();

private:
    void sendNormalizedCoefficients(TimeSeriesSamples coefficients);

    QImage IplImage2QImage(IplImage *iplImage);
    QImage cvMat2QImage(cv::Mat m, int width, int height);
    float maxMatf(cv::Mat m);

    CvCapture* capture;
    PCA pca;


};

#endif // PCAPLAYER_H
