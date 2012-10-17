#ifndef CVUTIL_H
#define CVUTIL_H

#include <opencv/cv.h>

class CVUtil
{
public:
    static unsigned int componentCount(cv::Mat m)
    {
        size_t count = 1;
        for(int d = 0; d < m.dims; d++)
            count *= m.size[d];
        return count;
    };

private:
    CVUtil();
};

#endif // CVUTIL_H
