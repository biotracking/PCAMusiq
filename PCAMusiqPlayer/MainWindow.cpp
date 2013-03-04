#include "MainWindow.h"

#include <QApplication>
#include <QFileInfo>
#include <QDebug>
#include <QtOpenGL/QGLWidget>

#include "LevelView.h"
#include "Config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsView(&scene)
{



    QObject::connect(
        &player, SIGNAL(displayNewSourceFrame(QImage)),
           this, SLOT(newVideoFrame(QImage))
    );
    QObject::connect(
        &player, SIGNAL(newReconstructedFrame(QImage)),
           this, SLOT(newReconstructedFrame(QImage))
    );
    QObject::connect(
        &player, SIGNAL(newCoefficients(PCACoefficients)),
           this, SLOT(newCoefficients(PCACoefficients)),
        Qt::DirectConnection
    );
    QObject::connect(
        &player, SIGNAL(newCoefficients(PCACoefficients)),
     &oscSender, SLOT(newValues(PCACoefficients)),
        Qt::DirectConnection
    );

    graphicsView.resize(1280, 900);

    //QVector<QImage> eigenImages = player.eigenFrames();

    singlePlotHeight = float(graphicsView.height()) / float(RELEVANT_COMPONENTS);


    //float eigenFrameAspect = float(eigenImages[0].width()) / float(eigenImages[0].height());
    //float eigenFrameDisplayWidth = singlePlotHeight * eigenFrameAspect;

    coefficientsPlot.setX(0.0);

    float nextY = 0.0;
    for(int p = 0; p < RELEVANT_COMPONENTS; p++)
    {
        LevelView* ts = new LevelView();

        coefficientLevelViews.push_back(ts);
        coefficientsPlot.addToGroup(ts);
        //ts->setPos(0.0, nextY);
        ts->setPos(0.0, singlePlotHeight * p);

        //QImage eigenImage = eigenImages[p];
        //QGraphicsPixmapItem* evFrameItem = new QGraphicsPixmapItem(QPixmap::fromImage(eigenImage));
        //evFrameItem->setScale(eigenFrameDisplayWidth / eigenImage.width());
        //evFrameItem->setPos(-eigenFrameDisplayWidth, singlePlotHeight * p);
        //coefficientsPlot.addToGroup(evFrameItem);

        nextY += ts->boundingRect().height();
    }



    scene.addItem(&videoPixmap);
    scene.addItem(&reconstructedPixmap);

    coefficientsPlot.setZValue(10);
    scene.addItem(&coefficientsPlot);

    QString labelString;
    for (int a = 0; a < QApplication::instance()->arguments().size(); a++)
    {
        QFileInfo info(QApplication::instance()->arguments()[a]);
        labelString = labelString + info.fileName() + ", ";
    }
    labelString.truncate(labelString.size()-2);


    graphicsView.setViewport(new QGLWidget());
    graphicsView.show();
    graphicsView.window()->setWindowTitle(labelString);

    player.start();

    this->hide();
}

void MainWindow::newCoefficients(PCACoefficients coefficients)
{
    //qDebug() << "hello from MainWindow::newCoefficients()";
    for(int c = 0; c < coefficients.size(); c++)
    {
        LevelView* ts = coefficientLevelViews[c];
        ts->setValue(coefficients[c]);
    }
}

void MainWindow::newVideoFrame(QImage frame)
{
    videoPixmap.setPixmap(QPixmap::fromImage(frame));
    videoPixmap.setScale(0.5 * float(graphicsView.height()) / float(frame.height()));


    for(int p = 0; p < coefficientLevelViews.size(); p++)
    {
        LevelView* ts = coefficientLevelViews[p];
        QRectF tsRect(0.0, 0.0, PLOT_WIDTH, singlePlotHeight);
        ts->setBoundingRect(tsRect);
        ts->setPos(- PLOT_WIDTH, singlePlotHeight * p);
    }
    graphicsView.fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::newReconstructedFrame(QImage frame)
{
    reconstructedPixmap.setPixmap(QPixmap::fromImage(frame));
    reconstructedPixmap.setY(0.5 * graphicsView.height());
    reconstructedPixmap.setScale(0.5 * float(graphicsView.height()) / float(frame.height()));
}

MainWindow::~MainWindow()
{

}
