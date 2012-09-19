#ifndef PCA_H
#define PCA_H

#include <QString>
#include <QMutex>

#include <opencv/cv.h>

#include "external/cnpy/cnpy.h"

class PCA
{
    //Q_OBJECT

public:
    PCA(QString path);
    ~PCA();
    std::vector<float> project(IplImage* img);
    cv::Mat backProject(std::vector<float> vector);


private:
    cv::Mat eigenVectors;
    cv::Mat mean;

    int evImageWidth;
    int evImageHeight;

    cv::PCA cvPCA;
    cv::Mat backProjection;

    //QMutex outputMutex;
};

#endif // PCA_H
