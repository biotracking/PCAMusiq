#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

#include <cv/cv.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "Opening " << QApplication::instance()->arguments()[1];
}

MainWindow::~MainWindow()
{

}
