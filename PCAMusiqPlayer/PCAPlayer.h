#ifndef PCAPLAYER_H
#define PCAPLAYER_H

#include <QObject>
#include <QThread>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoSource/VideoSource.h"

#include "LevelView.h"
#include "PCA.h"

typedef QVector<float> PCACoefficients;

class PCAPlayer : public VideoSourceFrameReceiver
{
    Q_OBJECT

public:
    friend class VideoSource;

signals:
    void displayNewSourceFrame(QImage frame);
    void newReconstructedFrame(QImage reconstructedFrame);
    void newCoefficients(PCACoefficients coefficients);
    void newNormalizedCoefficients(PCACoefficients coefficients);

public:
    PCAPlayer();
    void start();

    QVector<QImage> eigenFrames();

protected:
    // new frame callback, called by video source
    void newFrame(cv::Mat frame);

private:
    void sendNormalizedCoefficients(PCACoefficients coefficients);

    QImage IplImage2QImage(IplImage *iplImage);
    //QImage cvMat2QImage(cv::Mat m, int width = -1, int height = -1);
    float maxMatf(cv::Mat m);

    VideoSource* videoSource;

    PCA pca;


};

#endif // PCAPLAYER_H
