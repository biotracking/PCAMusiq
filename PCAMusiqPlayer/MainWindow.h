#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>

#include "PCAPlayer.h"
#include "OscSender.h"
#include "LevelView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel image;

    QGraphicsScene scene;
    QGraphicsView graphicsView;

    QGraphicsPixmapItem videoPixmap;
    QGraphicsPixmapItem reconstructedPixmap;
    QGraphicsItemGroup coefficientsPlot;
    std::vector<LevelView*> coefficientLevelViews;

    PCAPlayer player;
    OscSender oscSender;

    float singleIndicatorWidth;

public slots:
    void newVideoFrame(QImage frame);
    void newReconstructedFrame(QImage frame);
    void newCoefficients(PCACoefficients coefficients);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void layoutLevels();

protected:
    virtual void resizeEvent(QResizeEvent e);
};

#endif // MAINWINDOW_H
