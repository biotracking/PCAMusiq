#include "PCA.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include <opencv/cv.h>



PCA::PCA(QString path)
{


    qDebug() << "PCA's gonna open " << path;
    QDir pcaDir(path);
    QStringList files = pcaDir.entryList(QStringList("*.npy"));




    bool initializedEigenVectorMatrix = false;


    for (int v = 0; v < files.size(); v++)
    {
        QString file = files[v];

        QString fullPath = path + "/" + file;
        cnpy::NpyArray eigenVector = cnpy::npy_load(fullPath.toStdString().c_str());
        npArrays.push_back(eigenVector);
        float* data = reinterpret_cast<float*>(eigenVector.data);

        if(!initializedEigenVectorMatrix)
        {
            eigenVectors = cv::Mat(files.size(), eigenVector.shape[0], CV_MAT32F); // does this need a 3-channel type?
            qDebug() << "Made EV matrix of size " << files.size() << " x " << eigenVector.shape[0];
            initializedEigenVectorMatrix = true;
        }

        eigenVectors.row(v) = cv::Mat(1, eigenVector.shape[0], CV_MAT32F, data);
        //cv::Mat(1, eigenVector.shape[0], CV_MAT32F, data).copyTo(eigenVectors.row(v));

//        for(int c = 0; c < eigenVector.shape[0]; c++)
//        {
//            eigenVectors.row(v).setTo( data[c]
//        }
        //std::cerr << std::endl << "--------------------" << std::endl;

    }
}

void PCA::project(IplImage* img)
{
    /*
    IplImage* scaledImg = cvCreateImage(cvSize(img->width, img->height), 8, 3);
    cv::resize(img, scaledImg, cv::Size(evImageWidth, evImageHeight), 0, 0, cv::INTER_CUBIC);

    cv::OutputArray coefficients;
    cv::PCAProject(scaledImg, mean, eigenVectors, coefficients);
    */
}

PCA::~PCA()
{
    qDebug() << "deconstruct PCA";
}
