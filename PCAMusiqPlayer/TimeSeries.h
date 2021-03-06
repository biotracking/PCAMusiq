#ifndef LevelView_H
#define LevelView_H

#include <QGraphicsItem>

#include <vector>

//typedef std::vector<float> LevelViewSamples;
typedef QVector<float> LevelViewSamples;

class LevelView : public QGraphicsItem
{
public:
    LevelView();

    void appendSample(float value);

    QRectF boundingRect() const;
    void setBoundingRect(QRectF rect) { this->rect = rect; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//public slots:
//    void append(float value);
private:
    LevelViewSamples values;
    float max;
    float min;
    float currentValue;

    QRectF rect;
    float valueLabelWidth;
};

#endif // LevelView_H
