#include "OscSender.h"

#include <QDebug>
#include <QApplication>

#include "Config.h"

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
    for(int s = samples.size(); s < MUSIC_OSC_FLOAT_COUNT; s++)
    {
        samples.push_back(0.0);
    }

    lo_send(oscAppAddress, MUSIC_OSC_PATH, "ffffffffffff",
            samples[0], samples[1], samples[2],  samples[3],
            samples[4], samples[5], samples[6],  samples[7],
            samples[8], samples[9], samples[10], samples[11]);
}
