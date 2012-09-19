#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QtOpenGL/QGLWidget>

#include "TimeSeries.h"
#include "Config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsView(&scene)
{
    QObject::connect(&player, SIGNAL(newFrame(QImage)),
                        this, SLOT(newVideoFrame(QImage)));
    QObject::connect(&player, SIGNAL(newReconstructedFrame(QImage)),
                        this, SLOT(newReconstructedFrame(QImage)));
    QObject::connect(&player, SIGNAL(newCoefficients(TimeSeriesSamples)),
                        this, SLOT(newCoefficients(TimeSeriesSamples)),
                     Qt::DirectConnection);

    float nextY = 0.0;
    for(int p = 0; p < RELEVANT_COMPONENTS; p++)
    {
        TimeSeries* ts = new TimeSeries();
        //QGraphicsPathItem* plot = new QGraphicsPathItem(&coefficientsPlot);
        coefficientTimeSeries.push_back(ts);
        coefficientsPlot.addToGroup(ts);
        ts->setPos(0.0, nextY);
        nextY += ts->boundingRect().height();

        //QPainterPath path(QPointF(0.0, 0.0));
        //path.lineTo(100.0, 100.0);


        //plot->setBrush(QBrush(QColor(230, 50, 50
        //plot->setPath(path);
    }



    scene.addItem(&videoPixmap);
    scene.addItem(&reconstructedPixmap);

    //coefficientsPlot.setPos(1500.0, 0.0);
    coefficientsPlot.setZValue(10);
    scene.addItem(&coefficientsPlot);


    graphicsView.resize(1024, 768);
    graphicsView.setViewport(new QGLWidget());
    graphicsView.show();
    player.start();
}

void MainWindow::newCoefficients(TimeSeriesSamples coefficients)
{
    for(int c = 0; c < coefficients.size(); c++)
    {
        TimeSeries* ts = coefficientTimeSeries[c];
        ts->appendSample(coefficients[c]);
    }
}

void MainWindow::newVideoFrame(QImage frame)
{
    videoPixmap.setPixmap(QPixmap::fromImage(frame));
    coefficientsPlot.setPos(frame.width(), 0.0);
    graphicsView.fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    reconstructedPixmap.setPos(0.0, frame.height());
}

void MainWindow::newReconstructedFrame(QImage frame)
{
    reconstructedPixmap.setPixmap(QPixmap::fromImage(frame));
}

MainWindow::~MainWindow()
{

}
