#ifndef PCA_H
#define PCA_H

#include <QString>
#include <QMutex>

#include <opencv/cv.h>

#include "external/cnpy/cnpy.h"

class PCA
{
    //Q_OBJECT

    friend class PCAPlayer;

public:
    PCA(QString path);
    ~PCA();
    std::vector<float> project(cv::Mat img);
    cv::Mat backProject(std::vector<float> vector);

    int getEVImageWidth() { return evImageWidth; }
    int getEVImageHeight() { return evImageHeight; }

protected:
    cv::PCA cvPCA;

private:
    int evImageWidth;
    int evImageHeight;


    cv::Mat backProjection;

    // not part of state.
    // don't want OpenCV to keep allocating new matrix data, so keep these here.
    cv::Mat scaledImgMat;
    cv::Mat coefficients;
    cv::Mat imageVector;
    bool initializedImageVector;
};

#endif // PCA_H
