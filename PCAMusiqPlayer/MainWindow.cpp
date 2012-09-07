#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QtOpenGL/QGLWidget>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsView(&scene)
{

    QObject::connect(&player, SIGNAL(newFrame(QImage)),
                        this, SLOT(newVideoFrame(QImage)));

    scene.addItem(&videoPixmap);

    graphicsView.resize(1024, 768);
    graphicsView.setViewport(new QGLWidget());
    graphicsView.show();
    player.start();
}

void MainWindow::newVideoFrame(QImage frame)
{


    //qDebug() << "received video frame " << frame.width() << " x " << frame.height();
    //image.setPixmap(QPixmap::fromImage(frame));
    videoPixmap.setPixmap(QPixmap::fromImage(frame));
    //graphicsView.fitInView(&videoPixmap, Qt::KeepAspectRatio);
    //image.show();
}

MainWindow::~MainWindow()
{

}
