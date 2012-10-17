#include "TimeSeries.h"

#include <QDebug>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>

#include "MathUtil.h"

TimeSeries::TimeSeries()
{
    valueLabelWidth = 100.0;
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

    currentValue = value;
}

QRectF TimeSeries::boundingRect() const
{
    return rect;
}

void TimeSeries::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF plotRect = boundingRect();
    plotRect.setRight(plotRect.right() - valueLabelWidth);
    QRectF labelRect = boundingRect();
    labelRect.setLeft(plotRect.right());

    QPointF middleLeft(plotRect.left(), plotRect.height()/2.0);
    QPointF middleRight(plotRect.right(), plotRect.height()/2.0);


    painter->setPen(QColor(230, 230, 230));
    painter->drawLine(middleLeft, middleRight);

    painter->setPen(QColor(0, 0, 0));
    painter->drawRect(boundingRect());
    painter->drawRect(plotRect);
    //painter->drawLine(middleRight.x(), boundingRect().bottom(), middleRight.x(), boundingRect().top());

    painter->drawText(labelRect, QString("%1").arg(currentValue));


    if(values.size() < 1)
        return;



    unsigned int start = MAX(0, int(values.size() - plotRect.width()));
    //QPainterPath path(QPointF(rect.right(), plotRect.height() * (values.last() - min) / (max - min)));
    QPainterPath path(QPointF(plotRect.right(), plotRect.height() * (values.last() - min) / (max - min)));
    for(int s = values.size()-2; s >= 0; s--)
    {
        float normalized = (values[s] - min) / (max - min);
        //path.lineTo((float) (rect.right() - values.size() + s), plotRect.height() * normalized);
        path.lineTo((float) (plotRect.right() - values.size() + s), plotRect.height() * normalized);
    }



    painter->setPen(QColor(230, 50, 50));
    painter->drawPath(path);
}
