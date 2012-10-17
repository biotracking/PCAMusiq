#include "OscSender.h"

#include <QDebug>
#include <QApplication>

OscSender::OscSender()
{
    QString url = QApplication::instance()->arguments()[3];
    QStringList parts = url.split(QChar(':'));

    qDebug() << "Sending OSC to" << url;
    //QString port = QApplication::instance()->arguments()[3];

    oscAppAddress = lo_address_new(parts[0].toStdString().c_str(), parts[1].toStdString().c_str());
}

void OscSender::newValues(TimeSeriesSamples samples)
{
    qDebug() << samples[0];
}
