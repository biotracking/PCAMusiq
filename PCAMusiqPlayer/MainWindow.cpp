#include "MainWindow.h"
#include <QApplication>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QObject::connect(&player, SIGNAL(newFrame(QImage)),
                        this, SLOT(newVideoFrame(QImage)));

    player.start();
}

void MainWindow::newVideoFrame(QImage frame)
{
    //qDebug() << "received video frame " << frame.width() << " x " << frame.height();
    image.setPixmap(QPixmap::fromImage(frame));
    image.show();
}

MainWindow::~MainWindow()
{

}
