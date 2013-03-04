#include "OscSender.h"

#include <QDebug>
#include <QApplication>

#include "MathUtil.h"
#include "Config.h"

OscSender::OscSender()
{
    QString url = QApplication::instance()->arguments()[3];
    QStringList parts = url.split(QChar(':'));

    qDebug() << "Sending OSC to" << url;
    //QString port = QApplication::instance()->arguments()[3];

    oscAppAddress = lo_address_new(parts[0].toStdString().c_str(), parts[1].toStdString().c_str());
    processedFirstFrame = false;
}

void OscSender::newValues(PCACoefficients samples)
{
    for(int s = samples.size(); s < MUSIC_BOX_INT_COUNT; s++)
    {
        samples.push_back(0.0);
    }

    std::vector<int> musicBoxInts(samples.size());
    if(!processedFirstFrame)
    {
        minimums = samples;
        maximums = samples;
        processedFirstFrame = true;
    }
    for(int c = 0; c < samples.size(); c++)
    {
        minimums[c] = MIN(minimums[c], samples[c]);
        maximums[c] = MAX(maximums[c], samples[c]);
        musicBoxInts[c] = MUSIC_BOX_INT_MIN + (MUSIC_BOX_INT_MAX - MUSIC_BOX_INT_MIN) * (samples[c] - minimums[c]) / (maximums[c] - minimums[c]);
    }

    lo_send(oscAppAddress, MUSIC_BOX_OSC_PATH, "iiiiiiiiiiii",
            musicBoxInts[0], musicBoxInts[1], musicBoxInts[2],  musicBoxInts[3],
            musicBoxInts[4], musicBoxInts[5], musicBoxInts[6],  musicBoxInts[7],
            musicBoxInts[8], musicBoxInts[9], musicBoxInts[10], musicBoxInts[11]);
}
