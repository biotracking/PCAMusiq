#ifndef CVUTIL_H
#define CVUTIL_H

#include <QImage>
#include <QDebug>

#include <opencv/cv.h>

#include "MathUtil.h"

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

    template <typename T> static QImage cvMat2QImage(cv::Mat m, /*ColorFormat colorFormat,*/ int width = -1, int height = -1)
    {
        //qDebug() << "cvMat2QImage input: " << width << ", " << height;

        if(width == -1 || height == -1)
        {
            width = m.cols;
            height = m.rows;
        }


        //qDebug() << "depth: " << m.depth() << ", channels: " << m.channels(); CV_8U;

        QImage image = QImage(width, height, QImage::Format_RGB888);

        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                int pixelIndex = y * width + x;

    #ifdef BLACK_AND_WHITE
                float r = m.at<T>(pixelIndex);
                float g = r;
                float b = r;

    #else
                float b = m.at<T>(pixelIndex * 3 + 0);
                float g = m.at<T>(pixelIndex * 3 + 1);
                float r = m.at<T>(pixelIndex * 3 + 2);
    #endif

    #ifdef CLAMP_RECONSTRUCTED_COLOR
                // getting some r, g, b's outside 0.0 to 255.0 (surely because of the information loss from PCA)
                r = CLAMP(r, 0.0, 255.0);
                g = CLAMP(g, 0.0, 255.0);
                b = CLAMP(b, 0.0, 255.0);
    #endif

                image.setPixel(x, y, qRgb(r,g,b));
            }
        }


        return image;
    }

private:
    CVUtil();
};

#endif // CVUTIL_H
