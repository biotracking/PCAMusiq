#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "PCAPlayer.h"
#include "TimeSeries.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel image;

    QGraphicsScene scene;
    QGraphicsView graphicsView;

    QGraphicsPixmapItem videoPixmap;
    QGraphicsItemGroup coefficientsPlot;
    std::vector<TimeSeries*> coefficientTimeSeries;

    PCAPlayer player;

public slots:
    void newVideoFrame(QImage frame);
    void newCoefficients(TimeSeriesSamples coefficients);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
