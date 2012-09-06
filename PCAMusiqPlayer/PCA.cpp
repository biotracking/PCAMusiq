#include "PCA.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include "external/cnpy/cnpy.h"

PCA::PCA(QString path)
{
    qDebug() << "PCA's gonna open " << path;
    QDir pcaDir(path);
    QStringList files = pcaDir.entryList(QStringList("*.npy"));
    QString file;
    foreach (file, files)
    {
        QString fullPath = path + "/" + file;
        cnpy::NpyArray eigenVector = cnpy::npy_load(fullPath.toStdString().c_str());
        float* data = reinterpret_cast<float*>(eigenVector.data);
        for(int c = 0; c < 10; c++)
        {
            std::cerr << data[c] << ", ";
        }
        std::cerr << std::endl << "--------------------" << std::endl;

    }
}
