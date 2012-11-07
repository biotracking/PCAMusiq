#include "PCA.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QVariant>

#include <opencv/cv.h>

#include "Config.h"

#include "CVUtil.h"

cv::Mat npyFile2cvMat(std::string path)
{
    cnpy::NpyArray array = cnpy::npy_load(path.c_str());
    float* data = reinterpret_cast<float*>(array.data);

    int* sizes = new int[array.shape.size()];
    for(size_t d = 0; d < array.shape.size(); d++)
    {
        sizes[d] = array.shape[d];

    }

    cv::Mat result = cv::Mat(array.shape.size(), sizes, CV_MAT32F, data);

    delete[](sizes);

    return result;
}

cv::Mat vectorizeMat32f(cv::Mat m)
{
    return cv::Mat(1, CVUtil::componentCount(m), CV_MAT32F, m.data);
}

cv::Mat vectorizeMat(cv::Mat m)
{
    unsigned int values = m.channels();
    for(int d = 0; d < m.dims; d++)
        values *= m.size[d];

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
            cvPCA.mean = npyFile2cvMat((path + "/" + filename).toStdString());
            evImageWidth = cvPCA.mean.size[1];
            evImageHeight = cvPCA.mean.size[0];

            cvPCA.mean = vectorizeMat32f(cvPCA.mean);

            backProjection.copySize(cvPCA.mean);
        }
    }

    assert(evImageWidth > 0);

    for (int v = 0; v < eigenVectorFilenames.size() && v < RELEVANT_COMPONENTS; v++)
    {
        QString filePath = eigenVectorFilenames[v];
        QString fullPath = path + "/" + filePath;
        cv::Mat eigenVectorImage = npyFile2cvMat(fullPath.toStdString());
        cv::Mat eigenVector = vectorizeMat32f(eigenVectorImage);

        cvPCA.eigenvectors.push_back(eigenVector.row(0));
    }
}

std::vector<float> PCA::project(IplImage* img)
{
    cv::Mat imgMat(img);

    cv::Mat scaledImgMat;
    cv::resize(imgMat, scaledImgMat, cv::Size(evImageWidth, evImageHeight), 0, 0, cv::INTER_CUBIC);

    cv::Mat imageVector = vectorizeMat(scaledImgMat);

    cv::Mat coefficients;

    cvPCA.project(imageVector, coefficients);

    std::vector<float> result(RELEVANT_COMPONENTS);
    for(size_t c = 0; c < result.size(); c++)
    {
        result[c] = ((float*)coefficients.data)[c];
    }

    return result;
}

cv::Mat PCA::backProject(std::vector<float> vector)
{

    cvPCA.backProject(vector, backProjection);

    return backProjection;
}

PCA::~PCA()
{
    qDebug() << "deconstruct PCA";
}
