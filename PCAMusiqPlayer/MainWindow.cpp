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
    //coefficientsPlot.setScale(0.5);
    scene.addItem(&coefficientsPlot);

    QString labelString;
    for (int a = 0; a < QApplication::instance()->arguments().size(); a++)
    {
        labelString = labelString + QApplication::instance()->arguments()[a] + "\n";
    }
    labelString.truncate(labelString.size()-1);
    QGraphicsTextItem* label = new QGraphicsTextItem(labelString);
    QFont font = label->font();
    font.setPointSizeF(30);
    label->setFont(font);
    label->setPos(- label->boundingRect().width() / 2.0, -label->boundingRect().height());
    scene.addItem(label);


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

    //float timeSeriesScale = 2 * frame.height() / (coefficientTimeSeries.size() * coefficientTimeSeries[0]->boundingRect().height());
    float singlePlotHeight = 2 * frame.height() / RELEVANT_COMPONENTS;
    for(int p = 0; p < coefficientTimeSeries.size(); p++)
    {
        TimeSeries* ts = coefficientTimeSeries[p];
        QRectF tsRect(- PLOT_WIDTH, 0.0, PLOT_WIDTH, singlePlotHeight);
        ts->setBoundingRect(tsRect);
        ts->setPos(- PLOT_WIDTH, singlePlotHeight * p);
        //coefficientsPlot.setPos( coefficientsPlot.boundingRect().width(), p * );
    }
    //coefficientsPlot.setScale(timeSeriesScale);
    graphicsView.fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    qDebug() << scene.sceneRect().height();

    reconstructedPixmap.setPos(0.0, frame.height());
}

void MainWindow::newReconstructedFrame(QImage frame)
{
    reconstructedPixmap.setPixmap(QPixmap::fromImage(frame));
    reconstructedPixmap.setScale(videoPixmap.boundingRect().width() / frame.width());
}

MainWindow::~MainWindow()
{

}
