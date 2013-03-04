#ifndef OSCSENDER_H
#define OSCSENDER_H

#include <QObject>

#include "lo/lo.h"

#include "LevelView.h"
#include "PCAPlayer.h"

class OscSender : public QObject
{
    Q_OBJECT

public:
    OscSender();

public slots:
    void newValues(PCACoefficients samples);

private:
    lo_address oscAppAddress;

    PCACoefficients minimums;
    PCACoefficients maximums;
    bool processedFirstFrame;
};

#endif // OSCSENDER_H
