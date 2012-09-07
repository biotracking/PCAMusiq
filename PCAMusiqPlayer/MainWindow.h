#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "PCAPlayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel image;
    QGraphicsScene scene;
    QGraphicsView graphicsView;
    QGraphicsPixmapItem videoPixmap;

    PCAPlayer player;

public slots:
    void newVideoFrame(QImage frame);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
