#include "PCA.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QVariant>

#include <opencv/cv.h>

cv::Mat npyFile2cvMat(std::string path)
{
    cnpy::NpyArray array = cnpy::npy_load(path.c_str());
    float* data = reinterpret_cast<float*>(array.data);

    int* sizes = new int[array.shape.size()];
    for(int d = 0; d < array.shape.size(); d++)
    {
        sizes[d] = array.shape[d];
        //qDebug() << sizes[d];

    }

    cv::Mat result = cv::Mat(array.shape.size(), sizes, CV_MAT32F, data);
    //cv::Mat result = cv::Mat(array.shape[0], array.shape[1], CV_MAT32F, data);

    delete[](sizes);

    return result;
}

size_t componentCount(cv::Mat m)
{
    size_t count = 1;
    for(int d = 0; d < m.dims; d++)
        count *= m.size[d];
    return count;
}

cv::Mat vectorizeMat32f(cv::Mat m)
{
    return cv::Mat(1, componentCount(m), CV_MAT32F, m.data);
}

cv::Mat vectorizeMat888(cv::Mat m)
{
    unsigned int values = 3 * componentCount(m);
    //qDebug() << "vectorizeMat888 values: " << values;
    cv::Mat vector = cvCreateMat(1, values, CV_MAT32F);
    unsigned char* mData = (unsigned char*) m.data;
    float* vData = (float*) vector.data;
    for(unsigned int i = 0; i < values; i++)
    {
        vData[i] = float(mData[i]);
    }
    return vector;
}

PCA::PCA(QString path)
{
    //qDebug() << "PCA's gonna open " << path;
    QDir pcaDir(path);
    QStringList npyFilePaths = pcaDir.entryList(QStringList("*.npy"));
    QStringList eigenVectorFilenames;
    QString filename;
    foreach (filename, npyFilePaths)
    {
        if(filename != "mean.npy")
            eigenVectorFilenames.append(filename);
        else
        {
            mean = npyFile2cvMat((path + "/" + filename).toStdString());
            evImageWidth = mean.size[1];
            evImageHeight = mean.size[0];
            //qDebug() << "inferred ev size: " << evImageWidth << ", " << evImageHeight;

            mean = vectorizeMat32f(mean);
        }
    }

    bool initializedEigenVectorMatrix = false;


    for (int v = 0; v < eigenVectorFilenames.size(); v++)
    {
        QString filePath = eigenVectorFilenames[v];
        QString fullPath = path + "/" + filePath;
        cv::Mat eigenVectorImage = npyFile2cvMat(fullPath.toStdString());
        cv::Mat eigenVector = vectorizeMat32f(eigenVectorImage);
        //qDebug() << "eigenVector is " << eigenVector.rows << " x " << eigenVector.cols;

        eigenVectors.push_back(eigenVector.row(0));

    }

    /*for(int r = 0; r < eigenVectors.rows; r++)
    {
        for(int c = 0; c < 5; c++)
        {
            std::cerr << eigenVectors.row(r).col(c) << ", ";
        }
        std::cerr << std::endl;
    }*/
}

std::vector<float> PCA::project(IplImage* img)
{

    //qDebug() << "img " << img->width << ", " << img->height << ", " << img->depth << ", " << img->nChannels;

    //cv::Mat scaledImg = cvCreateImage(cvSize(img->width, img->height), 8, 3);
    cv::Mat imgMat(img);

    //qDebug() << "imgMat dims: " << imgMat.dims;

    cv::Mat scaledImgMat;
    cv::resize(imgMat, scaledImgMat, cv::Size(evImageWidth, evImageHeight), 0, 0, cv::INTER_CUBIC);

    //qDebug() << "scaledImgMat: " << scaledImgMat.rows << " x " << scaledImgMat.cols << ", " << scaledImgMat.elemSize();
    cv::Mat imageVector = vectorizeMat888(scaledImgMat);

    cv::Mat coefficients;

    //qDebug() << eigenVectors.rows << ", " << eigenVectors.cols;
    //qDebug() << "imageVector" << imageVector.dims << ", " << componentCount(imageVector) << ", " << imageVector.type();
    //qDebug() << mean.dims << ", " << componentCount(mean);

    //qDebug() << "mean.data: " << mean.data;
    //qDebug() << "eigenvectors.data" << eigenVectors.data;
    //qDebug() << "mean.rows: " << mean.rows << ", mean.cols" << mean.cols;
    //qDebug() << "data.rows: " << imageVector.rows << ", data.cols" << imageVector.cols;
    //qDebug() << "(mean.rows == 1 && mean.cols == data.cols)" << (mean.rows == 1 && mean.cols == imageVector.cols);
    //qDebug() << "(mean.cols == 1 && mean.rows == data.rows)" << (mean.cols == 1 && mean.rows == imageVector.rows);

    cv::PCAProject(imageVector, mean, eigenVectors, coefficients);

    std::vector<float> result(20);
    for(int c = 0; c < result.size(); c++)
    {
        result[c] = ((float*)coefficients.data)[c];
        std::cerr << result[c];
    }
    std::cerr << std::endl;
    return result;
}

PCA::~PCA()
{
    qDebug() << "deconstruct PCA";
}
