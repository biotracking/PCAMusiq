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


    singleIndicatorWidth = float(graphicsView.width()) / float(RELEVANT_COMPONENTS);

    coefficientsPlot.setX(0.0);

    float nextY = 0.0;
    for(int p = 0; p < RELEVANT_COMPONENTS; p++)
    {
        LevelView* ts = new LevelView();

        coefficientLevelViews.push_back(ts);
        coefficientsPlot.addToGroup(ts);
        //ts->setPos(0.0, nextY);
        ts->setPos(singleIndicatorWidth * p, 0.0);

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
    //graphicsView.show();
    this->setCentralWidget(&graphicsView);
    this->setWindowTitle(labelString);
    this->resize(1280, 800);


    player.start();

    //this->hide();
}

void MainWindow::newCoefficients(PCACoefficients coefficients)
{
    for(int c = 0; c < coefficients.size(); c++)
    {
        LevelView* ts = coefficientLevelViews[c];
        ts->setValue(coefficients[c]);
    }
}

void MainWindow::layoutLevels()
{
    singleIndicatorWidth = graphicsView.width() / RELEVANT_COMPONENTS;
    qDebug() << "graphicsView.height(): " << graphicsView.height();
    qDebug() << "videoPixmap.boundingRect().height(): " << videoPixmap.boundingRect().height();
    float height = graphicsView.height() - videoPixmap.boundingRect().height() * videoPixmap.scale();
    for(int p = 0; p < coefficientLevelViews.size(); p++)
    {
        LevelView* ts = coefficientLevelViews[p];
        //QRectF levelRect(singleIndicatorWidth * p, 0.0, singleIndicatorWidth, 100.0);
        QRectF levelRect(0.0, 0.0, singleIndicatorWidth, height);
        ts->setBoundingRect(levelRect);
        ts->setPos(singleIndicatorWidth * p, 0.0);
    }
    scene.setSceneRect(0, -videoPixmap.scale() * videoPixmap.boundingRect().height(), graphicsView.width(), graphicsView.height());
}

void MainWindow::newVideoFrame(QImage frame)
{
    float scale = 0.5 * float(graphicsView.width()) / float(frame.width());
    videoPixmap.setPixmap(QPixmap::fromImage(frame));
    videoPixmap.setScale(scale);
    videoPixmap.setY(- frame.height() * scale);

    layoutLevels();
}

void MainWindow::newReconstructedFrame(QImage frame)
{
    float scale = 0.5 * float(graphicsView.width()) / float(frame.width());
    reconstructedPixmap.setPixmap(QPixmap::fromImage(frame));
    reconstructedPixmap.setX(0.5 * graphicsView.width());
    reconstructedPixmap.setScale(scale);
    reconstructedPixmap.setY(- frame.height() * scale);
}

void MainWindow::resizeEvent(QResizeEvent e)
{
    qDebug() << "got resize event";
    layoutLevels();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case 'f': case 'F':
        if(this->isFullScreen())
        {
            this->setCursor(Qt::ArrowCursor);
            this->showNormal();
        }
        else
        {
            this->setCursor(Qt::BlankCursor);
            this->showFullScreen();
        }
        break;
    default: break;
    }
}

MainWindow::~MainWindow()
{

}
