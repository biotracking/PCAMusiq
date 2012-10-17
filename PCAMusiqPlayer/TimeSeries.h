#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <QGraphicsItem>

#include <vector>

//typedef std::vector<float> TimeSeriesSamples;
typedef QVector<float> TimeSeriesSamples;

class TimeSeries : public QGraphicsItem
{
public:
    TimeSeries();

    void appendSample(float value);

    QRectF boundingRect() const;
    void setBoundingRect(QRectF rect) { this->rect = rect; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//public slots:
//    void append(float value);
private:
    TimeSeriesSamples values;
    float max;
    float min;

    QRectF rect;
    float valueLabelWidth;
};

#endif // TIMESERIES_H
