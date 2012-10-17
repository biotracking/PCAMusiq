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
    std::vector<float> project(IplImage* img);
    cv::Mat backProject(std::vector<float> vector);

    int getEVImageWidth() { return evImageWidth; }
    int getEVImageHeight() { return evImageHeight; }

protected:
    cv::PCA cvPCA;

private:
    int evImageWidth;
    int evImageHeight;


    cv::Mat backProjection;
};

#endif // PCA_H
