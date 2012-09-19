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
    void newCoefficients(TimeSeriesSamples coefficients);

public:
    PCAPlayer();
    void run();

private:
    QImage IplImage2QImage(IplImage *iplImage);
};

#endif // PCAPLAYER_H
