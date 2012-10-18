#include "PCAPlayer.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QImage>


#include "Config.h"
#include "MathUtil.h"


PCAPlayer::PCAPlayer()
  : pca(QApplication::instance()->arguments()[2])
{
    // load original video
    QString videofilename = QApplication::instance()->arguments()[1];
    qDebug() << "Opening " << videofilename;
    capture = cvCaptureFromFile(videofilename.toStdString().c_str());
    cvQueryFrame(capture);

    // load PCA
    //QString pcaPath = QApplication::instance()->arguments()[2];


}

QVector<QImage> PCAPlayer::eigenFrames()
{
    QVector<QImage> images;
    for(int ev = 0; ev < pca.cvPCA.eigenvectors.rows; ev++)
    {
        cv::Mat eigenVector = pca.cvPCA.eigenvectors.row(ev);
        float max = maxMatf(eigenVector);
        cv::Mat visibleEigenVector = 255 * eigenVector / max;

        QImage image = this->cvMat2QImage(visibleEigenVector, pca.getEVImageWidth(), pca.getEVImageHeight());

        images.push_back(image);
    }
    return images;
}

void PCAPlayer::run()
{

    while(1)
    {
        IplImage* img = 0;
        if(!cvGrabFrame(capture)){              // capture a frame
          printf("Could not grab a frame\n\7");
          //exit(0);
        }
        img = cvRetrieveFrame(capture);           // retrieve the captured frame



#ifdef BLACK_AND_WHITE
        cv::Mat capturedFrame(img);
        cv::Mat capturedFrameBlackAndWhite(capturedFrame.rows, capturedFrame.cols, CV_8UC1);
        cv::cvtColor(capturedFrame, capturedFrameBlackAndWhite, CV_BGR2GRAY, 1);
        IplImage capturedFrameBlackAndWhiteIPL = (IplImage) capturedFrameBlackAndWhite;
        img = &capturedFrameBlackAndWhiteIPL;
#endif

        std::vector<float> pcaProjection = pca.project(img);
        QVector<float> coefficients(pcaProjection.size());
        for(size_t c = 0; c < pcaProjection.size(); c++)
        {
            coefficients[c] = pcaProjection[c];
        }
        newCoefficients(coefficients);

        QImage newImage = this->IplImage2QImage(img);
        newFrame(newImage);
        newReconstructedFrame(cvMat2QImage(pca.backProject(pcaProjection), pca.getEVImageWidth(), pca.getEVImageHeight()));
    }
}

size_t componentCount(cv::Mat m)
{
    size_t count = 1;
    for(int d = 0; d < m.dims; d++)
        count *= m.size[d];
    return count;
}

float PCAPlayer::maxMatf(cv::Mat m)
{
    float max = 0.0;

    for(int v = 0; v < componentCount(m); v++)
    {
        max = MAX(m.at<float>(v), max);
    }

    return max;
}

QImage PCAPlayer::cvMat2QImage(cv::Mat m, int width, int height)
{
    QImage image = QImage(width, height, QImage::Format_RGB888);


    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            int pixelIndex = y * width + x;

#ifdef BLACK_AND_WHITE
            float r = m.at<float>(pixelIndex);
            float g = r;
            float b = r;

#else
            float b = m.at<float>(pixelIndex * 3 + 0);
            float g = m.at<float>(pixelIndex * 3 + 1);
            float r = m.at<float>(pixelIndex * 3 + 2);
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
