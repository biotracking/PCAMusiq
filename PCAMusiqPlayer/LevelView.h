#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include <QGraphicsItem>

#include <vector>

class LevelView : public QGraphicsItem
{
public:
    LevelView();

    void setValue(float value);

    QRectF boundingRect() const;
    void setBoundingRect(QRectF rect) { this->rect = rect; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//public slots:
//    void append(float value);
private:
    //TimeSeriesSamples values;
    bool receivedFirstValue;
    float max;
    float min;
    float currentValue;

    QRectF rect;
    float valueLabelWidth;
};

#endif // LEVELVIEW_H
