#include "TimeSeries.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>

TimeSeries::TimeSeries()
{
}

void TimeSeries::appendSample(float value)
{
    values.push_back(value);
    qDebug() << "appended";
}

QRectF TimeSeries::boundingRect() const
{
    return QRectF(0.0, 0.0, 400.0, 400.0);
}

void TimeSeries::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(values.size() < 1)
        return;

    QPainterPath path(QPointF(0.0, values[0]));
    for(unsigned int s = 1; s < values.size(); s++)
    {
        path.lineTo((float)s, values[s]);
    }

    painter->setPen(QColor(230, 50, 50));
    painter->drawPath(path);
}
