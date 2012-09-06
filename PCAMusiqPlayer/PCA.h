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
    void project(IplImage* img);

private:
    std::vector<cnpy::NpyArray> npArrays;
    cv::Mat eigenVectors;
};

#endif // PCA_H
