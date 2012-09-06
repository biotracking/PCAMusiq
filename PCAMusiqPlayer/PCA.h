#ifndef PCA_H
#define PCA_H

#include <QString>

#include <opencv/cv.h>

#include "external/cnpy/cnpy.h"

class PCA
{
public:
    PCA(QString path);
    ~PCA();
    std::vector<float> project(IplImage* img);

private:
    cv::Mat eigenVectors;
};

#endif // PCA_H
