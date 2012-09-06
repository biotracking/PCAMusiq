#include "PCA.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

PCA::PCA(QString path)
{
    qDebug() << "PCA's gonna open " << path;
    QDir pcaDir(path);
    QStringList files = pcaDir.entryList(QStringList("*.npy"));
    QString file;
    foreach (file, files)
    {
        qDebug() << file;
    }
}
