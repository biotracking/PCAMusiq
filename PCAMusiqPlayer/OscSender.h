#ifndef OSCSENDER_H
#define OSCSENDER_H

#include <QObject>

#include "lo/lo.h"

#include "TimeSeries.h"

class OscSender : public QObject
{
    Q_OBJECT

public:
    OscSender();

public slots:
    void newValues(TimeSeriesSamples samples);

private:
    lo_address oscAppAddress;
};

#endif // OSCSENDER_H
