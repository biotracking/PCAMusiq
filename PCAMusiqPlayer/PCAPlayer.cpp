#include "PCAPlayer.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QImage>

#include "VideoSource/VideoSourceFile.h"
#include "VideoSource/VideoSourceProsilicaCamera.h"

#include "Config.h"
#include "MathUtil.h"
#include "CVUtil.h"


PCAPlayer::PCAPlayer()
  : pca(QApplication::instance()->arguments()[2])
{
    // load original video
    QString input = QApplication::instance()->arguments()[1];

    // create concrete video source object;
    if(input == "camera")
    {
        qDebug() << "Video Input: Prosilica GigE Camera";
        this->videoSource = new VideoSourceProsilicaCamera(this);
    }
    else
    {
        qDebug() << "Video Input: " << input;
        this->videoSource = new VideoSourceFile(input, this);
    }
}

void PCAPlayer::start()
{
    this->videoSource->start();
}

QVector<QImage> PCAPlayer::eigenFrames()
{
    QVector<QImage> images;
    for(int ev = 0; ev < pca.cvPCA.eigenvectors.rows; ev++)
    {
        cv::Mat eigenVector = pca.cvPCA.eigenvectors.row(ev);
        float max = maxMatf(eigenVector);
        cv::Mat visibleEigenVector = 255 * eigenVector / max;

        qDebug() << visibleEigenVector.depth();
        QImage image = CVUtil::cvMat2QImage<float>(visibleEigenVector, pca.getEVImageWidth(), pca.getEVImageHeight());

        images.push_back(image);
    }
    return images;
}

/*void PCAPlayer::run()
{

    while(1)
    { */
//
void PCAPlayer::newFrame(Frame_8UC3 _frame)
{
    cv::Mat frame(cv::Size(_frame.width, _frame.height), CV_8UC3, & _frame.data[0], cv::Mat::AUTO_STEP);

    std::cerr << "_frame.data.size(): " << _frame.data.size() << std::endl;
    std::vector<float> pcaProjection = pca.project(frame);
    QVector<float> coefficients(pcaProjection.size());
    for(size_t c = 0; c < pcaProjection.size(); c++)
    {
        coefficients[c] = pcaProjection[c];
    }

    //qDebug() << coefficients;
    newCoefficients(coefficients);

    QImage newImage = CVUtil::cvMat2QImage<unsigned char>(frame);
    displayNewSourceFrame(newImage);

    newReconstructedFrame(
        CVUtil::cvMat2QImage<float>(pca.backProject(pcaProjection)/*, colorFormat*/, pca.getEVImageWidth(), pca.getEVImageHeight())
    );
}
/*
    }
}*/

/*size_t componentCount(cv::Mat m)
{
    size_t count = 1;
    for(int d = 0; d < m.dims; d++)
        count *= m.size[d];
    return count;
}*/

float PCAPlayer::maxMatf(cv::Mat m)
{
    float max = 0.0;

    for(unsigned int v = 0; v < CVUtil::componentCount(m); v++)
    {
        max = MAX(m.at<float>(v), max);
    }

    return max;
}

QImage PCAPlayer::IplImage2QImage(IplImage *iplImage) {

    assert(iplImage->depth == IPL_DEPTH_8U);

    QImage image = QImage(iplImage->width, iplImage->height, QImage::Format_RGB888);
    QImage* qtImage = &image;

    //int channels = iplImage->nChannels;
    char *data = iplImage->imageData;
    char r, g, b;

    for (int y=0; y < qtImage->height(); y++, data += iplImage->widthStep)
    {
        for (int x=0; x < qtImage->width(); x++)
        {
            switch(iplImage->nChannels)
            {
            case 3:
                b = data[x * 3 + 0];
                g = data[x * 3 + 1];
                r = data[x * 3 + 2];
                qtImage->setPixel(x, y, qRgb(r,g,b));
                break;
            case 1:
                char v = data[x];
                qtImage->setPixel(x, y, qRgb(v, v, v));
                break;
            }
        }
    }

    return image;
}
