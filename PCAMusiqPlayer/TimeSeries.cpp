#include "TimeSeries.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a > b ? b : a)

TimeSeries::TimeSeries()
{
}

void TimeSeries::appendSample(float value)
{
    if(values.size() == 0)
    {
        max = value;
        min = value;
    }
    else
    {
        max = MAX(max, value);
        min = MIN(min, value);
    }
    values.push_back(value);
}

QRectF TimeSeries::boundingRect() const
{
    return QRectF(0.0, 0.0, 400.0, 100.0);
}

void TimeSeries::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(values.size() < 1)
        return;

    unsigned int start = MAX(0, int(values.size() - boundingRect().width()));
    QPainterPath path(QPointF(0.0, boundingRect().height() * (values[start] - min) / (max - min)));
    for(unsigned int s = start; s < values.size(); s++)
    {
        float normalized = (values[s] - min) / (max - min);
        path.lineTo((float) (s - start), boundingRect().height() * normalized);
    }

    painter->setPen(QColor(230, 50, 50));
    painter->drawPath(path);
}
