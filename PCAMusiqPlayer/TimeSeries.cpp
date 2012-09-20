#include "TimeSeries.h"

#include <QDebug>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>

#include "MathUtil.h"

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
    return rect;
}

void TimeSeries::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF middleLeft(boundingRect().left(), boundingRect().height()/2.0);
    QPointF middleRight(boundingRect().right(), boundingRect().height()/2.0);


    painter->setPen(QColor(230, 230, 230));
    painter->drawLine(middleLeft, middleRight);

    painter->setPen(QColor(0, 0, 0));
    painter->drawRect(boundingRect());


    if(values.size() < 1)
        return;



    unsigned int start = MAX(0, int(values.size() - boundingRect().width()));
    QPainterPath path(QPointF(rect.right(), boundingRect().height() * (values.last() - min) / (max - min)));
    for(int s = values.size()-2; s >= 0; s--)
    {
        float normalized = (values[s] - min) / (max - min);
        path.lineTo((float) (rect.right() - values.size() + s), boundingRect().height() * normalized);
    }



    painter->setPen(QColor(230, 50, 50));
    painter->drawPath(path);
}
