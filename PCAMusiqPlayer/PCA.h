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



private:
    cv::Mat eigenVectors;
    cv::Mat mean;

    int evImageWidth;
    int evImageHeight;

    QMutex outputMutex;
};

#endif // PCA_H
